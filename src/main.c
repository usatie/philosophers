/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 18:16:33 by susami            #+#    #+#             */
/*   Updated: 2022/10/19 01:38:24 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "philo.h"

// TODO: pthread_xxxのerror handling

#define ONE_SEC_IN_USEC 1000000
#define ONE_SEC_IN_MSEC 1000
#define ONE_MSEC_IN_USEC 1000

int	get_timestamp_ms(t_timeval since)
{
	t_timeval	now;
	int			ts_msec;

	gettimeofday(&now, NULL);
	ts_msec = (now.tv_sec - since.tv_sec) * ONE_SEC_IN_MSEC;
	ts_msec += (now.tv_usec - since.tv_usec) / ONE_MSEC_IN_USEC;
	return (ts_msec);
}

t_timeval	timeadd_msec(t_timeval t, int interval_msec)
{
	t.tv_sec += interval_msec / ONE_SEC_IN_MSEC;
	t.tv_usec += (interval_msec % ONE_SEC_IN_MSEC) * ONE_MSEC_IN_USEC;
	t.tv_sec += t.tv_usec / ONE_SEC_IN_USEC;
	t.tv_usec %= ONE_SEC_IN_USEC;
	t.tv_usec -= (t.tv_usec % ONE_MSEC_IN_USEC);
	return (t);
}

suseconds_t	timediff_usec(t_timeval start, t_timeval end)
{
	suseconds_t	diff;

	diff = (end.tv_sec - start.tv_sec) * ONE_SEC_IN_USEC;
	diff += (end.tv_usec - start.tv_usec);
	return (diff);
}

void	sleep_until(t_timeval t)
{
	t_timeval	now;
	suseconds_t	diff;

	gettimeofday(&now, NULL);
	diff = timediff_usec(now, t);
	while (diff > 0)
	{
		usleep(diff / 2);
		gettimeofday(&now, NULL);
		diff = timediff_usec(now, t);
	}
}

void	philo_log(t_philo *philo, char *msg)
{
	int	ts;

	pthread_mutex_lock(&philo->e->monitor.mtx);
	if (!philo->e->monitor.is_died || strcmp(msg, "died") == 0)
	{
		ts = get_timestamp_ms(philo->e->started_at);
		printf("%d %d %s\n", ts, philo->id, msg);
	}
	pthread_mutex_unlock(&philo->e->monitor.mtx);
}

void	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->left->mtx);
	philo_log(philo, "has taken a fork");
	pthread_mutex_lock(&philo->right->mtx);
	philo_log(philo, "has taken a fork");
	pthread_mutex_lock(&philo->mtx);
	gettimeofday(&philo->last_eat_at, NULL);
	philo->last_eat_at.tv_usec -= (philo->last_eat_at.tv_usec % ONE_MSEC_IN_USEC); // msec以下切り捨て
	philo->eat_count++;
	philo->state = PH_EATING;
	philo_log(philo, "is eating");
	pthread_mutex_unlock(&philo->mtx);
	sleep_until(timeadd_msec(philo->last_eat_at, philo->e->args.time_to_eat_ms));
	pthread_mutex_unlock(&philo->left->mtx);
	pthread_mutex_unlock(&philo->right->mtx);
}

// last_eat_atはphiloのスレッド以外からは更新されないので
// readだけならmutexの必要なし
void	philo_sleep(t_philo *philo)
{
	philo->state = PH_SLEEPING;
	philo_log(philo, "is sleeping");
	sleep_until(timeadd_msec(philo->last_eat_at, philo->e->args.time_to_eat_ms + philo->e->args.time_to_sleep_ms));
}

void	philo_think(t_philo *philo)
{
	philo_log(philo, "is thinking");
	philo->state = PH_THINKING;
	const int	n = philo->e->args.num_philo;
	const int	k = philo->e->args.num_philo / 2;
	const int	initial_slot = (k * philo->id) % n;
	if (philo->eat_count == 0)
	{
		sleep_until(timeadd_msec(philo->last_eat_at, philo->e->args.time_to_eat_ms * initial_slot / k));
	}
	else
		sleep_until(timeadd_msec(philo->last_eat_at, philo->e->args.time_to_eat_ms * n / k));
}

bool	should_continue(t_philo *philo)
{
	const t_args	*args = &philo->e->args;
	bool			continue_flg;

	continue_flg = ((args->max_eat < 0) || (philo->eat_count < args->max_eat));
	if (continue_flg)
	{
		pthread_mutex_lock(&philo->e->monitor.mtx);
		if (philo->e->monitor.is_died)
			continue_flg = false;
		pthread_mutex_unlock(&philo->e->monitor.mtx);
	}
	return (continue_flg);
}

void	err_exit(char *msg)
{
	write(2, msg, strlen(msg));
	exit(1);
}

void	*philosopher_func(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	sleep_until(philo->e->started_at);
	while (should_continue(philo))
	{
		if (philo->state == PH_THINKING)
		{
			philo_eat(philo);
		}
		else if (philo->state == PH_EATING)
		{
			philo_sleep(philo);
		}
		else if (philo->state == PH_SLEEPING)
		{
			philo_think(philo);
		}
		else
		{
			err_exit("Unknown State");
		}
	}
	return (NULL);
}

void	init_forks(t_env *e)
{
	int	i;

	memset(&e->forks, 0, sizeof(t_fork) * MAX_PHILO);
	i = 0;
	while (i < e->args.num_philo)
	{
		e->forks[i].id = i + 1;
		pthread_mutex_init(&e->forks[i].mtx, NULL);
		i++;
	}
}

void	init_philosophers(t_env *e)
{
	int	i;

	memset(&e->philosophers, 0, sizeof(t_philo) * MAX_PHILO);
	i = 0;
	while (i < e->args.num_philo)
	{
		e->philosophers[i].e = e;
		e->philosophers[i].id = i + 1;
		e->philosophers[i].last_eat_at = e->started_at;
		e->philosophers[i].state = PH_SLEEPING;
		if (i == 0)
			e->philosophers[i].left = &e->forks[e->args.num_philo - 1];
		else
			e->philosophers[i].left = &e->forks[i - 1];
		e->philosophers[i].right = &e->forks[i];
		pthread_mutex_init(&e->philosophers[i].mtx, NULL);
		i++;
	}
}

void	create_philosophers_threads(t_env *e)
{
	int	i;

	i = 0;
	while (i < e->args.num_philo)
	{
		pthread_create(&e->philosophers[i].tid, NULL, philosopher_func, &e->philosophers[i]);
		pthread_detach(e->philosophers[i].tid);
		i++;
	}
}

void	*monitor_philosophers(void *arg)
{
	t_env	*e;
	bool	all_alive = true;
	bool	some_still_eating = true;

	e = (t_env *)arg;
	sleep_until(e->started_at);
	while (all_alive && some_still_eating)
	{
		some_still_eating = false;
		// wait for threads to complete
		// Check if some philo is died or all philo has eaten max
		int	i;

		i = 0;
		while (all_alive && i < e->args.num_philo)
		{
			pthread_mutex_lock(&e->philosophers[i].mtx);
			if (e->args.max_eat < 0 || e->philosophers[i].eat_count < e->args.max_eat)
			{
				some_still_eating = true;
				t_timeval	now;
				gettimeofday(&now, NULL);
				now.tv_usec -= (now.tv_usec % ONE_MSEC_IN_USEC);
				suseconds_t	diff = timediff_usec(timeadd_msec(e->philosophers[i].last_eat_at, e->args.time_to_die_ms), now);
				if (diff > 0)
				{
					all_alive = false;
					pthread_mutex_lock(&e->monitor.mtx);
					e->monitor.is_died = true;
					pthread_mutex_unlock(&e->monitor.mtx);
					philo_log(&e->philosophers[i], "died");
				}
			}
			pthread_mutex_unlock(&e->philosophers[i].mtx);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}

void	create_monitor_thread(t_env *e)
{
	pthread_mutex_init(&e->monitor.mtx, NULL);
	pthread_create(&e->monitor.tid, NULL, monitor_philosophers, e);
}

void	cleanup_mutex(t_env *e)
{
	int	i;

	pthread_mutex_destroy(&e->monitor.mtx);
	i = 0;
	while (i < e->args.num_philo)
	{
		pthread_mutex_destroy(&e->philosophers[i].mtx);
		pthread_mutex_destroy(&e->forks[i].mtx);
		i++;
	}
}

void	argparse(t_args *args, int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	args->num_philo = 199;
	args->time_to_die_ms = 1990;
	args->time_to_eat_ms = 990;
	args->time_to_sleep_ms = 1000;
	args->max_eat = -1;
}

void	wait_simulation_ends(t_env *e)
{
	int	i;

	pthread_join(e->monitor.tid, NULL);
	i = 0;
	while (i < e->args.num_philo)
	{
		pthread_join(e->philosophers[i].tid, NULL);
		i++;
	}
	cleanup_mutex(e);
}

int	main(int argc, char *argv[])
{
	t_env	e;

	gettimeofday(&e.started_at, NULL);
	e.started_at.tv_sec += 1;
	e.started_at.tv_usec = 0;
	argparse(&e.args, argc, argv);
	init_forks(&e);
	init_philosophers(&e);
	create_philosophers_threads(&e);
	create_monitor_thread(&e);
	wait_simulation_ends(&e);
	return (0);
}
