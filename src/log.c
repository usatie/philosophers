/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 10:58:12 by susami            #+#    #+#             */
/*   Updated: 2022/10/19 23:33:56 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "philo.h"

// This method must be called from the philosopher's thread or lock 
// the philosopher's mutex before calling it. Because it reads
// `philo->last_eat_at` without locking mutex.
// Populate the timestamp when the log is flushed to tp if non-NULL.
int	philo_log(t_philo *philo, const char *msg, t_timeval *tp)
{
	t_timeval	now;
	int			ts;
	int			error;

	error = 0;
	pthread_mutex_lock(&philo->e->monitor.mtx);
	if (!philo->e->monitor.is_died)
	{
		if (is_philo_died(philo, &now))
		{
			error = -1;
			philo_log_died(philo, now);
		}
		else
		{
			ts = timediff_usec(philo->e->started_at, now) / 1000;
			printf("%d %d %s\n", ts, philo->id, msg);
			if (tp != NULL)
				*tp = now;
		}
	}
	else
		error = -2;
	pthread_mutex_unlock(&philo->e->monitor.mtx);
	return (error);
}

// Only flush once
// This function caller should lock monitor mutex before calling it.
void	philo_log_died(t_philo *philo, t_timeval t)
{
	static bool	flushed = false;
	int			ts;

	philo->e->monitor.is_died = true;
	if (!flushed)
	{
		ts = timediff_msec(philo->e->started_at, t);
		printf("%d %d died\n", ts, philo->id);
		flushed = true;
	}
}
