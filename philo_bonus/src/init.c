/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 09:07:28 by susami            #+#    #+#             */
/*   Updated: 2022/10/23 17:04:36 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "libftsubset.h"
#include "util.h"
#include "philo.h"

static void			deinit_semaphores(void);
static void			init_semaphores(t_env *e);
static void			init_philosophers(t_env *e);
static int			calc_optimal_interval_ms(t_args args);
static t_timeval	calc_initial_eat_at(int philo_id, t_env *e);

void	init_env(t_env *e)
{
	gettimeofday(&e->started_at, NULL);
	e->started_at.tv_sec += 1;
	e->started_at.tv_usec = 0;
	e->optimal_interval_ms = calc_optimal_interval_ms(e->args);
	init_philosophers(e);
	//deinit_semaphores();
	(void)deinit_semaphores;
	init_semaphores(e);
	printf("initialized.\n");
}

/* ************************************************************************** */
/*                                                                            */
/*                          File Private Functions                            */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>

#define SEM_FORKS "philosophers.forks"
#define SEM_WAITERS "philosophers.waiters"
#define SEM_MONITOR "philosophers.monitor"
#define SEM_PHILOSOPHERS "philosophers.philosophers."

sem_t	*sem_open_exit_on_err(const char *name, int value)
{
	const mode_t	perms = 0666;
	sem_t			*sem;

	sem = sem_open(name, O_CREAT, perms, value);
	if (sem == SEM_FAILED)
		err_exit("sem_open()");
	sem_unlink(name);
	return (sem);
}

static void	init_semaphores(t_env *e)
{
	int	i;

	e->forks = sem_open_exit_on_err(SEM_FORKS, e->args.num_philo);
	e->waiters = sem_open_exit_on_err(SEM_WAITERS, e->args.num_philo - 1);
	e->monitor.sem = sem_open_exit_on_err(SEM_MONITOR, 1);
	i = 0;
	while (i < e->args.num_philo)
	{
		char name[128] = SEM_PHILOSOPHERS;
		char *idstr;
		idstr = ft_itoa(e->philosophers[i].id);
		if (idstr == NULL)
			err_exit("ft_itoa()");
		ft_strlcat(name, idstr, 128);
		e->philosophers[i].sem = sem_open_exit_on_err(name, 1);
		i++;
	}
}

static void	deinit_semaphores(void)
{
	int	i;

	sem_unlink("philosophers.monitor");
	sem_unlink("philosophers.forks");
	sem_unlink("philosophers.waiters");
	i = 0;
	while (i < MAX_PHILO)
	{
		char name[128] = "philosophers.philosophers.";
		char id[4] = "";
		id[0] = '0' + i / 100;
		id[1] = '0' + (i % 100) / 10;
		id[2] = '0' + i % 10;
		id[3] = '\0';
		strlcat(name, id, 128);
		sem_unlink(name);
		i++;
	}
}

static void	init_philosophers(t_env *e)
{
	int	i;

	memset(&e->philosophers, 0, sizeof(t_philo) * MAX_PHILO);
	i = 0;
	while (i < e->args.num_philo)
	{
		e->philosophers[i].e = e;
		e->philosophers[i].id = i + 1;
		e->philosophers[i].last_eat_at = e->started_at;
		e->philosophers[i].last_sleep_at = e->started_at;
		e->philosophers[i].next_eat_at = e->started_at;//calc_initial_eat_at(i + 1, e);
		(void)calc_initial_eat_at;
		e->philosophers[i].state = PH_SLEEPING;
		i++;
	}
}

/*
Assume that (n = 2k) or (n = 2k + 1),
Optimal interval is time_to_eat_ms * n / k.

cf.
https://docs.google.com/spreadsheets/d/
1AhzUHrM78HUHS4OZG_cDI13fuQUasPWBPAhIN3c2GdU
*/
/*
Code below didn't work so commented out.
```
	// Use half of buffer to decrease delay.
	// Keep the other half as a buffer not to die.
	if (interval < args.time_to_die_ms)
		interval = (interval + args.time_to_die_ms) / 2;
```
*/
static int	calc_optimal_interval_ms(t_args args)
{
	const int	n = args.num_philo;
	const int	k = args.num_philo / 2;
	int			interval;

	interval = args.time_to_eat_ms * n / k;
	if (interval < args.time_to_eat_ms + args.time_to_sleep_ms)
		interval = args.time_to_eat_ms + args.time_to_sleep_ms;
	return (interval);
}

// If N is odd, N = 2k + 1, initial usleep should be (k * id) / N
// If N is even, N = 2k, initial usleep should be (k * id) / N
static t_timeval	calc_initial_eat_at(int philo_id, t_env *e)
{
	const int	time_to_eat_ms = e->args.time_to_eat_ms;
	const int	n = e->args.num_philo;
	const int	k = e->args.num_philo / 2;
	const int	initial_slot = (k * philo_id) % n;

	return (timeadd_msec(e->started_at, time_to_eat_ms * initial_slot / k));
}
