/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 09:07:28 by susami            #+#    #+#             */
/*   Updated: 2022/10/24 16:07:57 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <sys/time.h>
#include "util.h"
#include "philo.h"

#define SEMAPHORE_LOG       "log"
#define SEMAPHORE_LOG_DEAD  "log_dead"

static int	calc_optimal_interval_ms(t_args args);
static void	init_philosophers(t_env *e);

void	init_env(t_env *e)
{
	gettimeofday(&e->started_at, NULL);
	e->started_at.tv_sec += 1;
	e->started_at.tv_usec = 0;
	e->optimal_interval_ms = calc_optimal_interval_ms(e->args);
	e->forks = sem_open_exit_on_err(SEMAPHORE_LOG, e->args.num_philo);
	e->waiters = sem_open_exit_on_err(SEMAPHORE_LOG, e->args.num_philo - 1);
	e->log = sem_open_exit_on_err(SEMAPHORE_LOG, 1);
	e->log_dead = sem_open_exit_on_err(SEMAPHORE_LOG_DEAD, 1);
	init_philosophers(e);
}

/* ************************************************************************** */
/*                                                                            */
/*                          File Private Functions                            */
/*                                                                            */
/* ************************************************************************** */

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
		e->philosophers[i].next_eat_at = calc_initial_eat_at(i + 1, e);
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
