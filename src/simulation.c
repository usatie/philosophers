/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 11:10:57 by susami            #+#    #+#             */
/*   Updated: 2022/10/19 11:12:03 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	start_simulation(t_env *e)
{
	create_philosophers_threads(e);
	create_monitor_thread(e);
}

void	create_philosophers_threads(t_env *e)
{
	int	i;

	i = 0;
	while (i < e->args.num_philo)
	{
		pthread_create(&e->philosophers[i].tid, NULL, philosopher_func, &e->philosophers[i]);
		//pthread_detach(e->philosophers[i].tid);
		i++;
	}
}

void	create_monitor_thread(t_env *e)
{
	pthread_mutex_init(&e->monitor.mtx, NULL);
	pthread_create(&e->monitor.tid, NULL, monitor_func, e);
}

void	cleanup_mutex(t_env *e)
{
	int	i;

	pthread_mutex_destroy(&e->monitor.mtx);
	i = 0;
	while (i < e->args.num_philo)
	{
		pthread_mutex_destroy(&e->philosophers[i].mtx);
		pthread_mutex_destroy(&e->forks[i].mtx);
		i++;
	}
}

void	wait_simulation_ends(t_env *e)
{
	int	i;

	pthread_join(e->monitor.tid, NULL);
	i = 0;
	while (i < e->args.num_philo)
	{
		pthread_join(e->philosophers[i].tid, NULL);
		i++;
	}
	cleanup_mutex(e);
}