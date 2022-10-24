/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 13:49:42 by susami            #+#    #+#             */
/*   Updated: 2022/10/24 23:02:22 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "simulation.h"

static void	log_dead(t_philo *philo, int ts);
static bool	is_alive(t_philo *philo, t_timeval t);
static bool	is_hungry(t_philo *philo);
void		assert_alive(t_philo *philo, t_timeval *tp);

// Exit if philosopher is dead.
// Caller needs to wait `log` semaphore before calling this method.
// Additionally `self`, if caller is not the philo's thread.
// Populate the timestamp to tp when checked the philo is alive.
void	assert_alive(t_philo *philo, t_timeval *tp)
{
	t_timeval	now;
	int			ts;

	gettimeofday_rounddown_ms(&now);
	if (is_alive(philo, now))
	{
		if (tp)
			*tp = now;
		return ;
	}
	if (is_hungry(philo))
	{
		ts = timediff_msec(philo->e->started_at, now);
		log_dead(philo, ts);
		exit(EXIT_FAILURE);
	}
}

// When philo is dead, never release the log/log_dead lock so that 
// others will not print any message after dead message.
void	log_action(t_philo *philo, char *s, t_timeval *tp, t_logfunc *f)
{
	t_timeval	now;
	int			ts;

	sem_wait_exit_on_err(philo->e->log);
	assert_alive(philo, &now);
	ts = timediff_msec(philo->e->started_at, now);
	printf("%d %d %s\n", ts, philo->id, s);
	if (tp)
		*tp = now;
	if (f)
		f(philo);
	sem_post_exit_on_err(philo->e->log);
}

/* ************************************************************************** */
/*                                                                            */
/*                          File Private Functions                            */
/*                                                                            */
/* ************************************************************************** */

// When philo is dead, never release the log/log_dead semaphore so that 
// others will not print any message after dead message.
static void	log_dead(t_philo *philo, int ts)
{
	sem_wait_exit_on_err(philo->e->log_dead);
	printf("%d %d died\n", ts, philo->id);
}

static bool	is_hungry(t_philo *philo)
	__attribute__((disable_sanitizer_instrumentation))
{
	const int	eat_count = philo->eat_count;
	const int	max_eat = philo->e->args.max_eat;

	if (max_eat < 0)
		return (true);
	else
		return (eat_count < max_eat);
}

static bool	is_alive(t_philo *philo, t_timeval t)
{
	const int	elapsed = timediff_msec(philo->last_eat_at, t);

	if (elapsed <= philo->e->args.time_to_die_ms)
		return (true);
	else
		return (false);
}
