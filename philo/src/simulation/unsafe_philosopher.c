/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unsafe_philosopher.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 20:31:37 by susami            #+#    #+#             */
/*   Updated: 2022/10/20 21:41:20 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "util.h"
#include "philo.h"

static void	internal_log_dead(t_philo *philo, t_timeval t);

/*
   These functions must be called from the philosopher's thread.
   If calling thread is not the philosopher's, it should lock the
   philosopher's mutex before calling it. Because it reads
   `philo->last_eat_at` without locking mutex.
*/

// Populate the timestamp when philo's life is checked to tp if non-NULL.
bool	unsafe_is_dead(t_philo *philo, t_timeval *tp)
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

bool	unsafe_is_hungry(t_philo *philo)
{
	const t_args	*args = &philo->e->args;

	if (args->max_eat < 0)
		return (true);
	return (philo->eat_count < args->max_eat);
}

// Populate the timestamp when the log is flushed to tp if non-NULL.
int	unsafe_log_action(t_philo *philo, const char *msg, t_timeval *tp)
{
	t_timeval	now;
	int			ts;
	int			error;

	error = 0;
	pthread_mutex_lock(&philo->e->monitor.mtx);
	if (!philo->e->monitor.is_dead)
	{
		error = unsafe_is_dead(philo, &now);
		if (error == 0)
		{
			ts = timediff_msec(philo->e->started_at, now);
			printf("%d %d %s\n", ts, philo->id, msg);
			if (tp != NULL)
				*tp = now;
		}
		else
			internal_log_dead(philo, now);
	}
	else
		error = -1;
	pthread_mutex_unlock(&philo->e->monitor.mtx);
	return (error);
}

// Dead log is flushed only once
void	unsafe_log_dead(t_philo *philo, t_timeval t)
{
	pthread_mutex_lock(&philo->e->monitor.mtx);
	internal_log_dead(philo, t);
	pthread_mutex_unlock(&philo->e->monitor.mtx);
}

/* ************************************************************************** */
/*                                                                            */
/*                          File Private Functions                            */
/*                                                                            */
/* ************************************************************************** */

// Dead log is flushed only once
static void	internal_log_dead(t_philo *philo, t_timeval t)
{
	static bool	flushed = false;
	int			ts;

	philo->e->monitor.is_dead = true;
	if (!flushed)
	{
		ts = timediff_msec(philo->e->started_at, t);
		printf("%d %d died\n", ts, philo->id);
		flushed = true;
	}
}
