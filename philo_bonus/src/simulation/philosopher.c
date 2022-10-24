/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 09:45:25 by susami            #+#    #+#             */
/*   Updated: 2022/10/23 22:23:22 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdio.h>
#include "util.h"
#include "simulation.h"

static int	pickup_forks(t_philo *philo);
static int	philo_eat(t_philo *philo);
static int	philo_sleep(t_philo *philo);
static int	philo_think(t_philo *philo);

int	philosopher_func(t_philo *philo)
{
	int		error;

	error = 0;
	usleep_until(philo->e->started_at);
	while (error == 0 && unsafe_is_hungry(philo))
	{
		if (philo->state == PH_THINKING)
			error = philo_eat(philo);
		else if (philo->state == PH_EATING)
			error = philo_sleep(philo);
		else if (philo->state == PH_SLEEPING)
			error = philo_think(philo);
		else
			error = 1;
	}
	printf("philo %d will exit(%d)\n", philo->id, error);
	return (error);
}

/* ************************************************************************** */
/*                                                                            */
/*                          File Private Functions                            */
/*                                                                            */
/* ************************************************************************** */

static int	pickup_forks(t_philo *philo)
{
	int	error;

	sem_wait_exit_on_err(philo->e->waiters);
	sem_wait_exit_on_err(philo->e->forks);
	error = unsafe_log_action(philo, "has taken a fork", NULL);
	if (error == 0)
	{
		sem_wait_exit_on_err(philo->e->forks);
		error = unsafe_log_action(philo, "has taken a fork", NULL);
		if (error != 0)
		{
			sem_post(philo->e->forks);
			sem_post(philo->e->forks);
		}
	}
	else
		sem_post(philo->e->forks);
	sem_post(philo->e->waiters);
	return (error);
}

static int	philo_eat(t_philo *philo)
{
	int	error;

	error = 0;
	philo->state = PH_EATING;
	error = pickup_forks(philo);
	if (error == 0)
	{
		sem_wait(philo->sem);
		philo->eat_count++;
		error = unsafe_log_action(philo, "is eating", &philo->last_eat_at);
		sem_post(philo->sem);
		philo->next_eat_at = timeadd_msec(
				philo->last_eat_at,
				philo->e->optimal_interval_ms);
		if (error == 0)
			msleep_since(philo->last_eat_at, philo->e->args.time_to_eat_ms);
		sem_post(philo->e->forks);
		sem_post(philo->e->forks);
	}
	return (error);
}

// last_eat_atはphiloのスレッド以外からは更新されないので
// readだけならmutexの必要なし
static int	philo_sleep(t_philo *philo)
{
	int			error;
	const int	time_to_sleep_ms = philo->e->args.time_to_sleep_ms;

	philo->state = PH_SLEEPING;
	error = unsafe_log_action(philo, "is sleeping", &philo->last_sleep_at);
	if (error == 0)
		msleep_since(philo->last_sleep_at, time_to_sleep_ms);
	return (error);
}

// If N is odd, N = 2k + 1, initial usleep should be (k * id) / N
// If N is even, N = 2k, initial usleep should be (k * id) / N
static int	philo_think(t_philo *philo)
{
	int			error;

	philo->state = PH_THINKING;
	error = unsafe_log_action(philo, "is thinking", NULL);
	if (error == 0)
		usleep_until(philo->next_eat_at);
	return (error);
}
