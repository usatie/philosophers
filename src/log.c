/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 10:58:12 by susami            #+#    #+#             */
/*   Updated: 2022/10/19 21:16:14 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "philo.h"

int	philo_log(t_philo *philo, char *msg)
{
	int	ts;
	int	error;

	error = 0;
	pthread_mutex_lock(&philo->e->monitor.mtx);
	if (!philo->e->monitor.is_died)
	{
		ts = get_timestamp_ms(philo->e->started_at);
		printf("%d %d %s\n", ts, philo->id, msg);
	}
	else
		error = -1;
	pthread_mutex_unlock(&philo->e->monitor.mtx);
	return (error);
}

// Only flush once
void	philo_log_died(t_philo *philo)
{
	static bool	flushed = false;
	int			ts;

	pthread_mutex_lock(&philo->e->monitor.mtx);
	if (!flushed)
	{
		ts = get_timestamp_ms(philo->e->started_at);
		printf("%d %d died\n", ts, philo->id);
		flushed = true;
	}
	pthread_mutex_unlock(&philo->e->monitor.mtx);
}
