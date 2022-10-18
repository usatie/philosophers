/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 18:16:33 by susami            #+#    #+#             */
/*   Updated: 2022/10/18 15:56:01 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <string.h>
#include <stdbool.h>

// TODO: pthread_xxxのerror handling
#define MAX_PHILO 200

typedef struct s_fork		t_fork;
typedef struct s_philo		t_philo;
typedef struct s_monitor	t_monitor;
typedef struct s_args		t_args;
typedef struct s_env		t_env;
void	*philosopher_func(void *arg);
void	init_forks(t_env *e);
void	init_philosophers(t_env *e);
void	create_philosophers_threads(t_env *e);
void	cleanup_threads(t_env *e);
void	*monitor_philosophers(void *arg);
void	argparse(t_args *args, int argc, char *argv[]);
void	create_monitor_thread(t_env *e);
void	wait_simulation_ends(t_env *e);

struct s_fork {
	pthread_mutex_t	mtx;
	int				id;
};

struct s_philo {
	pthread_t		tid;
	pthread_mutex_t	mtx;
	int				id;
	t_env			*e;
	t_fork			*left;
	t_fork			*right;
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
};

void	*philosopher_func(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_detach(philo->tid);
	return (NULL);
}

void	init_philosophers(t_env *e)
{
	int	i;

	memset(&e->philosophers, 0, sizeof(t_philo) * MAX_PHILO);
	i = 0;
	while (i < e->args.num_philo)
	{
		e->philosophers[i].id = i + 1;
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

void	create_philosophers_threads(t_env *e)
{
	int	i;

	i = 0;
	while (i < e->args.num_philo)
	{
		pthread_create(&e->philosophers[i].tid, NULL, philosopher_func, &e->philosophers[i]);
		i++;
	}
}

void	*monitor_philosophers(void *arg)
{
	bool	all_alive = false;
	bool	some_still_eating = false;

	(void)arg;
	while (all_alive && some_still_eating)
	{
		// wait for threads to complete
		// Check if some philo is died or all philo has eaten max
	}
	return (NULL);
}

void	create_monitor_thread(t_env *e)
{
	pthread_create(&e->monitor.tid, NULL, monitor_philosophers, e);
}

void	cleanup_threads(t_env *e)
{
	int	i;

	i = 0;
	while (i < e->args.num_philo)
	{
		pthread_mutex_destroy(&e->philosophers[i].mtx);
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
	pthread_join(e->monitor.tid, NULL);
	cleanup_threads(e);
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
