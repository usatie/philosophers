/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 10:58:12 by susami            #+#    #+#             */
/*   Updated: 2022/10/19 11:02:01 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "philo.h"

static int	ft_strcmp(const char *s1, const char *s2);

int	philo_log(t_philo *philo, char *msg)
{
	int	ts;
	int	error;

	error = 0;
	pthread_mutex_lock(&philo->e->monitor.mtx);
	if (!philo->e->monitor.is_died || ft_strcmp(msg, "died") == 0)
	{
		ts = get_timestamp_ms(philo->e->started_at);
		printf("%d %d %s\n", ts, philo->id, msg);
	}
	else
		error = -1;
	pthread_mutex_unlock(&philo->e->monitor.mtx);
	return (error);
}

static int	ft_strcmp(const char *s1, const char *s2)
{
	size_t				i;
	const unsigned char	*u1;
	const unsigned char	*u2;

	u1 = (const unsigned char *)s1;
	u2 = (const unsigned char *)s2;
	i = 0;
	while (u1[i] || u2[i])
	{
		if (u1[i] != u2[i])
			return (u2[i] - u1[i]);
		i++;
	}
	return (0);
}
