/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 14:15:42 by susami            #+#    #+#             */
/*   Updated: 2022/10/24 18:58:22 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "util.h"
#include "simulation.h"

static bool	is_hungry(t_philo *philo);

void	*philosopher_func(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	usleep_until(philo->next_eat_at);
	while (is_hungry(philo))
	{
		if (philo->state == PH_THINKING)
			philo_eat(philo);
		else if (philo->state == PH_EATING)
			philo_sleep(philo);
		else if (philo->state == PH_SLEEPING)
			philo_think(philo);
		else
			err_exit("Unknown philosopher state");
	}
	putdown_forks(philo);
	exit(EXIT_SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                          File Private Functions                            */
/*                                                                            */
/* ************************************************************************** */

static bool	is_hungry(t_philo *philo)
{
	const int	eat_count = philo->eat_count;
	const int	max_eat = philo->e->args.max_eat;

	if (max_eat < 0)
		return (true);
	else
		return (eat_count < max_eat);
}
