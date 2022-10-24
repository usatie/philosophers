/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 09:57:05 by susami            #+#    #+#             */
/*   Updated: 2022/10/23 21:43:46 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "util.h"
#include "simulation.h"

#define MONITOR_INTERVAL_USEC 1000

static bool	should_continue_simulation(t_env *e);

int	monitor_func(t_env *e)
{
	int			status;
	pid_t		pid;
	t_timeval	now;
	t_philo		*philo;

	philo = NULL;
	usleep_until(e->started_at);
	pid = waitpid(0, &status, 0);
	sem_wait(e->monitor.sem);
	gettimeofday_rounddown_ms(&now);
	for (int i = 0; i < e->args.num_philo; i++)
	{
		if (pid == e->philosophers[i].pid)
		{
			philo = &e->philosophers[i];
			break ;
		}
	}
	unsafe_log_dead(philo, now);
	printf("Finish monitor\n");
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                          File Private Functions                            */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

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
		sem_wait(e->philosophers[i].sem);
		if (unsafe_is_hungry(&e->philosophers[i]))
		{
			eating = true;
			if (unsafe_is_dead(&e->philosophers[i], &now))
			{
				printf("unsafe_log_dead from monitor.\n");
				unsafe_log_dead(&e->philosophers[i], now);
				is_dead_any = true;
			}
		}
		sem_post(e->philosophers[i].sem);
		i++;
	}
	return (!is_dead_any && eating);
}
