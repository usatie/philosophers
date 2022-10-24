/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 14:15:42 by susami            #+#    #+#             */
/*   Updated: 2022/10/24 15:36:28 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "util.h"
#include "simulation.h"

static bool	is_hungry(t_philo *philo);
static void	eat(t_philo *philo);
static void	sleep(t_philo *philo);
static void	think(t_philo *philo);

void	*philosopher_func(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	usleep_until(philo->e->started_at);
	while (is_hungry(philo))
	{
		if (philo->state == PH_THINKING)
			eat(philo);
		else if (philo->state == PH_EATING)
			sleep(philo);
		else if (philo->state == PH_SLEEPING)
			think(philo);
		else
			err_exit("Unknown philosopher state");
	}
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

	return (eat_count < max_eat);
}

static void	eat(t_philo *philo)
{
	philo->state = PH_EATING;
	log_action(philo, "is eating", NULL, NULL);
}

static void	sleep(t_philo *philo)
{
	philo->state = PH_SLEEPING;
	log_action(philo, "is sleeping", NULL, NULL);
}

static void	think(t_philo *philo)
{
	philo->state = PH_THINKING;
	log_action(philo, "is thinking", NULL, NULL);
}
