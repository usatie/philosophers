/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 11:09:31 by susami            #+#    #+#             */
/*   Updated: 2022/10/19 11:10:03 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "philo.h"

void	*monitor_func(void *arg)
{
	t_env	*e;
	bool	all_alive = true;
	bool	some_still_eating = true;

	e = (t_env *)arg;
	usleep_until(e->started_at);
	while (all_alive && some_still_eating)
	{
		some_still_eating = false;
		// wait for threads to complete
		// Check if some philo is died or all philo has eaten max
		int	i;

		i = 0;
		while (all_alive && i < e->args.num_philo)
		{
			pthread_mutex_lock(&e->philosophers[i].mtx);
			if (is_hungry(&e->philosophers[i]))
			{
				some_still_eating = true;
				t_timeval	now;
				gettimeofday_rounddown_ms(&now);
				suseconds_t	diff = timediff_usec(timeadd_msec(e->philosophers[i].last_eat_at, e->args.time_to_die_ms), now);
				if (diff > 0)
				{
					all_alive = false;
					pthread_mutex_lock(&e->monitor.mtx);
					e->monitor.is_died = true;
					pthread_mutex_unlock(&e->monitor.mtx);
					philo_log(&e->philosophers[i], "died");
				}
			}
			pthread_mutex_unlock(&e->philosophers[i].mtx);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
