/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deinit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 18:56:52 by susami            #+#    #+#             */
/*   Updated: 2022/10/25 19:17:51 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "util.h"
#include "philo.h"

static void	close_all_semaphores(t_env *e);

void	deinit_env(t_env *e)
{
	close_all_semaphores(e);
}

/* ************************************************************************** */
/*                                                                            */
/*                          File Private Functions                            */
/*                                                                            */
/* ************************************************************************** */

static void	close_all_semaphores(t_env *e)
{
	int	i;

	sem_close_exit_on_err(e->forks);
	sem_close_exit_on_err(e->waiters);
	sem_close_exit_on_err(e->log);
	sem_close_exit_on_err(e->log_dead);
	i = 0;
	while (i < e->args.num_philo)
	{
		sem_close_exit_on_err(e->philosophers[i].self);
		i++;
	}
}
