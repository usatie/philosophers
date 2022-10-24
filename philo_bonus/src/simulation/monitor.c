/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 14:29:45 by susami            #+#    #+#             */
/*   Updated: 2022/10/24 16:35:57 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "util.h"
#include "simulation.h"

#define MONITOR_INTERVAL_USEC 1000

void	*monitor_func(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	usleep_until(philo->e->started_at);
	while (true)
	{
		sem_wait_exit_on_err(philo->e->log);
		sem_wait_exit_on_err(philo->self);
		assert_alive(philo, NULL);
		sem_post_exit_on_err(philo->self);
		sem_post_exit_on_err(philo->e->log);
		usleep(MONITOR_INTERVAL_USEC);
	}
}
