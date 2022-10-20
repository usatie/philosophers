/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unsafe_philosopher.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 20:31:37 by susami            #+#    #+#             */
/*   Updated: 2022/10/20 20:48:36 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include "philo.h"

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
