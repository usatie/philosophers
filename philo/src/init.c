/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 11:06:06 by susami            #+#    #+#             */
/*   Updated: 2022/10/21 19:12:14 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include "util.h"
#include "philo.h"

static void			init_forks(t_env *e);
static void			init_philosophers(t_env *e);
static int			calc_optimal_interval_ms(t_args args);
static t_timeval	calc_initial_eat_at(int philo_id, t_env *e);

void	init_env(t_env *e)
{
	gettimeofday(&e->started_at, NULL);
	e->started_at.tv_sec += 1;
	e->started_at.tv_usec = 0;
	e->optimal_interval_ms = calc_optimal_interval_ms(e->args);
	init_forks(e);
	init_philosophers(e);
}

/* ************************************************************************** */
/*                                                                            */
/*                          File Private Functions                            */
/*                                                                            */
/* ************************************************************************** */

static void	init_forks(t_env *e)
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
		if (i == 0)
		{
			e->philosophers[i].low = &e->forks[0];
			e->philosophers[i].high = &e->forks[e->args.num_philo - 1];
		}
		else
		{
			e->philosophers[i].low = &e->forks[i - 1];
			e->philosophers[i].high = &e->forks[i];
		}
		pthread_mutex_init(&e->philosophers[i].mtx, NULL);
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
