/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 11:09:31 by susami            #+#    #+#             */
/*   Updated: 2022/10/20 11:32:19 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "philo.h"

#include <stdio.h>

#define MONITOR_INTERVAL_USEC 1000

// This method must be called from the philosopher's thread or lock 
// the philosopher's mutex before calling it. Because it reads
// `philo->last_eat_at` without locking mutex.
// Populate the timestamp when philo's life is checked to tp if non-NULL.
bool	is_dead_no_philo_lock(t_philo *philo, t_timeval *tp)
{
	const int		time_to_die_ms = philo->e->args.time_to_die_ms;
	const t_timeval	deadline = timeadd_msec(philo->last_eat_at, time_to_die_ms);
	t_timeval		now;
	suseconds_t		diff;

	gettimeofday_rounddown_ms(&now);
	if (tp != NULL)
		*tp = now;
	diff = timediff_usec(deadline, now);
	return (diff > 0);
}

static bool	should_continue_simulation(t_env *e)
{
	t_timeval	now;
	bool		is_dead;
	bool		eating;
	int			i;

	is_dead = false;
	eating = false;
	i = 0;
	while (!is_dead && i < e->args.num_philo)
	{
		pthread_mutex_lock(&e->philosophers[i].mtx);
		if (is_hungry(&e->philosophers[i]))
		{
			eating = true;
			if (is_dead_no_philo_lock(&e->philosophers[i], &now))
			{
				philo_log_died(&e->philosophers[i], now);
				is_dead = true;
			}
		}
		pthread_mutex_unlock(&e->philosophers[i].mtx);
		i++;
	}
	return (!is_dead && eating);
}

void	*monitor_func(void *arg)
{
	t_env	*e;
	bool	should_continue;

	e = (t_env *)arg;
	should_continue = true;
	usleep_until(e->started_at);
	while (should_continue)
	{
		usleep(MONITOR_INTERVAL_USEC);
		should_continue = should_continue_simulation(e);
	}
	return (NULL);
}
