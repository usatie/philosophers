/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 11:09:31 by susami            #+#    #+#             */
/*   Updated: 2022/10/19 14:43:20 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "philo.h"

#define MONITOR_INTERVAL_USEC 1000

static bool	is_philo_died(t_philo *philo)
{
	const int		time_to_die_ms = philo->e->args.time_to_die_ms;
	const t_timeval	deadline = timeadd_msec(philo->last_eat_at, time_to_die_ms);
	t_timeval		now;
	suseconds_t		diff;
	t_monitor		*monitor;

	monitor = &philo->e->monitor;
	gettimeofday_rounddown_ms(&now);
	diff = timediff_usec(deadline, now);
	if (diff > 0)
	{
		pthread_mutex_lock(&monitor->mtx);
		monitor->is_died = true;
		pthread_mutex_unlock(&monitor->mtx);
		philo_log(philo, "died");
		return (true);
	}
	return (false);
}

static bool	should_continue_simulation(t_env *e)
{
	bool		died;
	bool		eating;
	int			i;

	died = false;
	eating = false;
	i = 0;
	while (!died && i < e->args.num_philo)
	{
		pthread_mutex_lock(&e->philosophers[i].mtx);
		if (is_hungry(&e->philosophers[i]))
		{
			eating = true;
			if (is_philo_died(&e->philosophers[i]))
				died = true;
		}
		pthread_mutex_unlock(&e->philosophers[i].mtx);
		i++;
	}
	return (!died && eating);
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
