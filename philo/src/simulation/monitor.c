/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 11:09:31 by susami            #+#    #+#             */
/*   Updated: 2022/10/20 22:17:00 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "util.h"
#include "simulation.h"

#define MONITOR_INTERVAL_USEC 1000

static bool	should_continue_simulation(t_env *e);

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

/* ************************************************************************** */
/*                                                                            */
/*                          File Private Functions                            */
/*                                                                            */
/* ************************************************************************** */

static bool	should_continue_simulation(t_env *e)
{
	t_timeval	now;
	bool		is_dead_any;
	bool		eating;
	int			i;

	is_dead_any = false;
	eating = false;
	i = 0;
	while (!is_dead_any && i < e->args.num_philo)
	{
		pthread_mutex_lock(&e->philosophers[i].mtx);
		if (unsafe_is_hungry(&e->philosophers[i]))
		{
			eating = true;
			if (unsafe_is_dead(&e->philosophers[i], &now))
			{
				unsafe_log_dead(&e->philosophers[i], now);
				is_dead_any = true;
			}
		}
		pthread_mutex_unlock(&e->philosophers[i].mtx);
		i++;
	}
	return (!is_dead_any && eating);
}
