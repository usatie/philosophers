/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 11:06:06 by susami            #+#    #+#             */
/*   Updated: 2022/10/19 11:07:49 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include "philo.h"

void	init_env(t_env *e)
{
	gettimeofday(&e->started_at, NULL);
	e->started_at.tv_sec += 1;
	e->started_at.tv_usec = 0;
	init_forks(e);
	init_philosophers(e);
}

void	init_forks(t_env *e)
{
	int	i;

	memset(&e->forks, 0, sizeof(t_fork) * MAX_PHILO);
	i = 0;
	while (i < e->args.num_philo)
	{
		e->forks[i].id = i + 1;
		pthread_mutex_init(&e->forks[i].mtx, NULL);
		i++;
	}
}

void	init_philosophers(t_env *e)
{
	int	i;

	memset(&e->philosophers, 0, sizeof(t_philo) * MAX_PHILO);
	i = 0;
	while (i < e->args.num_philo)
	{
		e->philosophers[i].e = e;
		e->philosophers[i].id = i + 1;
		e->philosophers[i].last_eat_at = e->started_at;
		e->philosophers[i].state = PH_SLEEPING;
		if (i == 0)
			e->philosophers[i].left = &e->forks[e->args.num_philo - 1];
		else
			e->philosophers[i].left = &e->forks[i - 1];
		e->philosophers[i].right = &e->forks[i];
		pthread_mutex_init(&e->philosophers[i].mtx, NULL);
		i++;
	}
}
