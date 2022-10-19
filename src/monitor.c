/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 11:09:31 by susami            #+#    #+#             */
/*   Updated: 2022/10/19 23:33:49 by susami           ###   ########.fr       */
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
bool	is_philo_died(t_philo *philo, t_timeval *tp)
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

// monitor.is_died is only written by monitor thread.
// So reading it is thread safe.
// (i.e.) Doesn't require mutex to read it.
static bool	should_continue_simulation(t_env *e)
{
	t_timeval	now;
	bool		eating;
	int			i;

	eating = false;
	i = 0;
	while (!e->monitor.is_died && i < e->args.num_philo)
	{
		pthread_mutex_lock(&e->philosophers[i].mtx);
		if (is_hungry(&e->philosophers[i]))
		{
			eating = true;
			if (is_philo_died(&e->philosophers[i], &now))
			{
				pthread_mutex_lock(&e->monitor.mtx);
				philo_log_died(&e->philosophers[i], now);
				pthread_mutex_unlock(&e->monitor.mtx);
			}
		}
		pthread_mutex_unlock(&e->philosophers[i].mtx);
		i++;
	}
	return (!e->monitor.is_died && eating);
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
