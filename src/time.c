/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 10:51:38 by susami            #+#    #+#             */
/*   Updated: 2022/10/19 10:55:25 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/time.h>
#include "philo.h"

#define ONE_SEC_IN_USEC 1000000
#define ONE_SEC_IN_MSEC 1000
#define ONE_MSEC_IN_USEC 1000

int	get_timestamp_ms(t_timeval since)
{
	t_timeval	now;
	int			diff;
	int			ts_msec;

	gettimeofday(&now, NULL);
	diff = (int)(now.tv_sec - since.tv_sec);
	ts_msec = diff * ONE_SEC_IN_MSEC;
	diff = (now.tv_usec - since.tv_usec);
	ts_msec += diff / ONE_MSEC_IN_USEC;
	return (ts_msec);
}

// msec以下切り捨て
void	gettimeofday_rounddown_ms(t_timeval *t)
{
	gettimeofday(t, NULL);
	t->tv_usec -= (t->tv_usec % ONE_MSEC_IN_USEC);
}

t_timeval	timeadd_msec(t_timeval t, int interval_msec)
{
	t.tv_sec += interval_msec / ONE_SEC_IN_MSEC;
	t.tv_usec += (interval_msec % ONE_SEC_IN_MSEC) * ONE_MSEC_IN_USEC;
	t.tv_sec += t.tv_usec / ONE_SEC_IN_USEC;
	t.tv_usec %= ONE_SEC_IN_USEC;
	t.tv_usec -= (t.tv_usec % ONE_MSEC_IN_USEC);
	return (t);
}

suseconds_t	timediff_usec(t_timeval start, t_timeval end)
{
	suseconds_t	diff;

	diff = (suseconds_t)(end.tv_sec - start.tv_sec) * ONE_SEC_IN_USEC;
	diff += (end.tv_usec - start.tv_usec);
	return (diff);
}

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
