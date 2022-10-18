/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 18:16:33 by susami            #+#    #+#             */
/*   Updated: 2022/10/18 18:12:41 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

// TODO: pthread_xxxのerror handling
#define MAX_PHILO 200

typedef struct timeval t_timeval;
typedef struct s_fork		t_fork;
typedef struct s_philo		t_philo;
typedef struct s_monitor	t_monitor;
typedef struct s_args		t_args;
typedef struct s_env		t_env;
void	*philosopher_func(void *arg);
void	init_forks(t_env *e);
void	init_philosophers(t_env *e);
void	create_philosophers_threads(t_env *e);
void	cleanup_mutex(t_env *e);
void	*monitor_philosophers(void *arg);
void	argparse(t_args *args, int argc, char *argv[]);
void	create_monitor_thread(t_env *e);
void	wait_simulation_ends(t_env *e);

struct s_fork {
	pthread_mutex_t	mtx;
	int				id;
};

enum e_pstate {
	PH_EATING,
	PH_SLEEPING,
	PH_THINKING,
};

struct s_philo {
	pthread_t		tid;
	pthread_mutex_t	mtx;
	int				id;
	t_env			*e;
	t_fork			*left;
	t_fork			*right;
	int				eat_count;
	t_timeval		last_eat_at;
	enum e_pstate	state;
	int				time_to_cool_down;
};

struct s_monitor {
	pthread_t		tid;
	pthread_mutex_t	mtx;
	bool			is_died;
};

struct s_args {
	int	num_philo;
	int	time_to_die_ms;
	int	time_to_eat_ms;
	int	time_to_sleep_ms;
	int	max_eat;
};

struct s_env {
	t_monitor	monitor;
	t_philo		philosophers[MAX_PHILO];
	t_fork		forks[MAX_PHILO];
	t_args		args;
	t_timeval	started_at;
};

void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);

void	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->left->mtx);
	pthread_mutex_lock(&philo->right->mtx);
	pthread_mutex_lock(&philo->mtx);
	gettimeofday(&philo->last_eat_at, NULL);
	philo->eat_count++;
	philo->time_to_cool_down--;
	pthread_mutex_unlock(&philo->mtx);
	sleep_until(last_eat_at + time_to_eat_ms);
	pthread_mutex_unlock(&philo->left->mtx);
	pthread_mutex_unlock(&philo->right->mtx);
}

// last_eat_atはphiloのスレッド以外からは更新されないので
// readだけならmutexの必要なし
void	philo_sleep(t_philo *philo)
{
	sleep_until(last_eat_at + time_to_eat_ms + time_to_sleep_ms);
}

void	philo_think(t_philo *philo)
{
	if (philo->e->args.num_philo & 1 && philo->time_to_cool_down == 0)
	{
		sleep_until(last_eat_at + time_to_eat_ms * 2);
		philo->time_to_cool_down = philo->e->args.num_philo / 2;
	}
	else
	{
		sleep_until(last_eat_at + time_to_eat_ms);
	}
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
	while (should_continue(philo))
	{
		if (philo->state == PH_THINKING)
			philo_eat(philo);
		else if (philo->state == PH_EATING)
			philo_sleep(philo);
		else if (philo->state == PH_SLEEPING)
			philo_think(philo);
		else
			err_exit("Unknown State");
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
		e->philosophers[i].id = i + 1;
		if (i & 1)
			e->philosophers[i].state = PH_THINKING;
		else
			e->philosophers[i].state = PH_SLEEPING;
		e->philosophers[i].time_to_cool_down = (i + 1) / 2;
		if (i == 0)
			e->philosophers[i].left = &e->forks[e->args.num_philo - 1];
		else
			e->philosophers[i].left = &e->forks[i - 1];
		if (i == e->args.num_philo - 1)
			e->philosophers[i].right = &e->forks[0];
		else
			e->philosophers[i].right = &e->forks[i + 1];
		pthread_mutex_init(&e->philosophers[i].mtx, NULL);
		i++;
	}
}

// Create odd index thread first. Because they use forks at first.
void	create_philosophers_threads(t_env *e)
{
	int	i;

	i = 1;
	while (i < e->args.num_philo)
	{
		pthread_create(&e->philosophers[i].tid, NULL, philosopher_func, &e->philosophers[i]);
		pthread_detach(e->philosophers[i].tid);
		i += 2;
	}
	i = 0;
	while (i < e->args.num_philo)
	{
		pthread_create(&e->philosophers[i].tid, NULL, philosopher_func, &e->philosophers[i]);
		pthread_detach(e->philosophers[i].tid);
		i += 2;
	}
}

void	*monitor_philosophers(void *arg)
{
	t_env	*e;
	bool	all_alive = false;
	bool	some_still_eating = false;

	e = (t_env *)arg;
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
			if (e->philosophers[i].eat_count < e->args.max_eat)
			{
				some_still_eating = true;
				last_eat_at = e->philosophers[i].last_eat_at;
				if (last_eat_at + time_to_die > now)
				{
					all_alive = false;
					pthread_mutex_lock(&e->monitor.mtx);
					e->monitor.is_died = true;
					pthread_mutex_unlock(&e->monitor.mtx);
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
	args->num_philo = 6;
	args->time_to_die_ms = 300;
	args->time_to_eat_ms = 100;
	args->time_to_sleep_ms = 100;
	args->max_eat = 3;
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

	argparse(&e.args, argc, argv);
	init_forks(&e);
	init_philosophers(&e);
	create_philosophers_threads(&e);
	create_monitor_thread(&e);
	wait_simulation_ends(&e);
	return (0);
}
