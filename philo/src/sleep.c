/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleep.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 15:01:20 by susami            #+#    #+#             */
/*   Updated: 2022/10/19 15:01:43 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "philo.h"

void	usleep_until(t_timeval t)
{
	t_timeval	now;
	suseconds_t	diff;

	gettimeofday(&now, NULL);
	diff = timediff_usec(now, t);
	while (diff > 0)
	{
		usleep((useconds_t)(diff / 2));
		gettimeofday(&now, NULL);
		diff = timediff_usec(now, t);
	}
}

void	msleep_since(t_timeval since, int milliseconds)
{
	t_timeval	end;

	end = timeadd_msec(since, milliseconds);
	usleep_until(end);
}
