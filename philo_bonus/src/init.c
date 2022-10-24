/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 09:07:28 by susami            #+#    #+#             */
/*   Updated: 2022/10/24 18:53:40 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "libftsubset.h"
#include "util.h"
#include "philo.h"

#define SEMAPHORE_LOG       "log"
#define SEMAPHORE_LOG_DEAD  "log_dead"
#define SEMAPHORE_PHILO     "philosopher"
#define SEMAPHORE_WAITERS   "waiters"
#define SEMAPHORE_FORKS     "forks"
#define MAX_NAME_LEN 128

static int			calc_optimal_interval_ms(t_args args);
static t_timeval	calc_initial_eat_at(int philo_id, t_env *e);
static void			init_philosophers(t_env *e);

void	init_env(t_env *e)
{
	const int	num_waiters = e->args.num_philo - 1;

	gettimeofday(&e->started_at, NULL);
	e->started_at.tv_sec += 1;
	e->started_at.tv_usec = 0;
	e->optimal_interval_ms = calc_optimal_interval_ms(e->args);
	e->forks = sem_open_exit_on_err(SEMAPHORE_FORKS, e->args.num_philo);
	if (num_waiters > 0)
		e->waiters = sem_open_exit_on_err(SEMAPHORE_WAITERS, num_waiters);
	else
		e->waiters = sem_open_exit_on_err(SEMAPHORE_WAITERS, 1);
	e->log = sem_open_exit_on_err(SEMAPHORE_LOG, 1);
	e->log_dead = sem_open_exit_on_err(SEMAPHORE_LOG_DEAD, 1);
	init_philosophers(e);
}

/* ************************************************************************** */
/*                                                                            */
/*                          File Private Functions                            */
/*                                                                            */
/* ************************************************************************** */

static char	*get_sem_name(int philo_id)
{
	char	*name;
	char	*id_str;

	name = ft_calloc(MAX_NAME_LEN, sizeof(char));
	if (name == NULL)
		err_exit("ft_calloc()");
	id_str = ft_itoa(philo_id);
	if (id_str == NULL)
		err_exit("ft_itoa()");
	ft_strlcat(name, SEMAPHORE_PHILO, MAX_NAME_LEN);
	ft_strlcat(name, id_str, MAX_NAME_LEN);
	free(id_str);
	return (name);
}

static void	init_philosophers(t_env *e)
{
	int		i;
	char	*name;

	memset(&e->philosophers, 0, sizeof(t_philo) * MAX_PHILO);
	i = 0;
	while (i < e->args.num_philo)
	{
		name = get_sem_name(i + 1);
		e->philosophers[i].e = e;
		e->philosophers[i].id = i + 1;
		e->philosophers[i].last_eat_at = e->started_at;
		e->philosophers[i].last_sleep_at = e->started_at;
		e->philosophers[i].next_eat_at = calc_initial_eat_at(i + 1, e);
		e->philosophers[i].state = PH_THINKING;
		e->philosophers[i].self = sem_open_exit_on_err(name, 1);
		free(name);
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

	if (k == 0)
		return (args.time_to_eat_ms + args.time_to_sleep_ms);
	interval = args.time_to_eat_ms * n / k;
	if (interval < args.time_to_eat_ms + args.time_to_sleep_ms)
		interval = args.time_to_eat_ms + args.time_to_sleep_ms;
	return (interval);
}

// On each 1/k time_to_eat_ms frame, 1 philosopher start eating.
static t_timeval	calc_initial_eat_at(int philo_id, t_env *e)
{
	const int	time_to_eat_ms = e->args.time_to_eat_ms;
	const int	k = e->args.num_philo / 2;

	if (k == 0)
		return (e->started_at);
	return (timeadd_msec(e->started_at, time_to_eat_ms * (philo_id - 1) / k));
}
