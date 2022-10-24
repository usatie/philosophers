/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 09:07:28 by susami            #+#    #+#             */
/*   Updated: 2022/10/24 12:52:48 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include "util.h"
#include "philo.h"

static int			calc_optimal_interval_ms(t_args args);

void	init_env(t_env *e)
{
	gettimeofday(&e->started_at, NULL);
	e->started_at.tv_sec += 1;
	e->started_at.tv_usec = 0;
	e->optimal_interval_ms = calc_optimal_interval_ms(e->args);
}

/* ************************************************************************** */
/*                                                                            */
/*                          File Private Functions                            */
/*                                                                            */
/* ************************************************************************** */

/*
Assume that (n = 2k) or (n = 2k + 1),
Optimal interval is time_to_eat_ms * n / k.

cf.
https://docs.google.com/spreadsheets/d/
1AhzUHrM78HUHS4OZG_cDI13fuQUasPWBPAhIN3c2GdU
*/
/*
Code below didn't work so commented out.
```
	// Use half of buffer to decrease delay.
	// Keep the other half as a buffer not to die.
	if (interval < args.time_to_die_ms)
		interval = (interval + args.time_to_die_ms) / 2;
```
*/
static int	calc_optimal_interval_ms(t_args args)
{
	const int	n = args.num_philo;
	const int	k = args.num_philo / 2;
	int			interval;

	interval = args.time_to_eat_ms * n / k;
	if (interval < args.time_to_eat_ms + args.time_to_sleep_ms)
		interval = args.time_to_eat_ms + args.time_to_sleep_ms;
	return (interval);
}
