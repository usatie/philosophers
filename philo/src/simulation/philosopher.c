/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 11:02:53 by susami            #+#    #+#             */
/*   Updated: 2022/10/20 22:17:21 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include "simulation.h"

static int	pickup_forks(t_philo *philo);
static int	philo_eat(t_philo *philo);
static int	philo_sleep(t_philo *philo);
static int	philo_think(t_philo *philo);

void	*philosopher_func(void *arg)
{
	int		error;
	t_philo	*philo;

	error = 0;
	philo = (t_philo *)arg;
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
		{
			err_exit("Unknown State");
		}
	}
	return (NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                          File Private Functions                            */
/*                                                                            */
/* ************************************************************************** */

static int	pickup_forks(t_philo *philo)
{
	int	error;

	pthread_mutex_lock(&philo->low->mtx);
	error = unsafe_log_action(philo, "has taken a fork", NULL);
	if (philo->low->id == philo->high->id)
		error = -1;
	if (error == 0)
	{
		pthread_mutex_lock(&philo->high->mtx);
		error = unsafe_log_action(philo, "has taken a fork", NULL);
		if (error)
			pthread_mutex_unlock(&philo->high->mtx);
	}
	if (error)
		pthread_mutex_unlock(&philo->low->mtx);
	return (error);
}

static int	philo_eat(t_philo *philo)
{
	int	error;

	philo->state = PH_EATING;
	error = pickup_forks(philo);
	if (error == 0)
	{
		pthread_mutex_lock(&philo->mtx);
		philo->eat_count++;
		error = unsafe_log_action(philo, "is eating", &philo->last_eat_at);
		pthread_mutex_unlock(&philo->mtx);
		if (error == 0)
			msleep_since(philo->last_eat_at, philo->e->args.time_to_eat_ms);
		pthread_mutex_unlock(&philo->high->mtx);
		pthread_mutex_unlock(&philo->low->mtx);
	}
	return (error);
}

// last_eat_atはphiloのスレッド以外からは更新されないので
// readだけならmutexの必要なし
static int	philo_sleep(t_philo *philo)
{
	int			error;
	const int	time_to_eat_ms = philo->e->args.time_to_eat_ms;
	const int	time_to_sleep_ms = philo->e->args.time_to_sleep_ms;

	philo->state = PH_SLEEPING;
	error = unsafe_log_action(philo, "is sleeping", NULL);
	if (error == 0)
		msleep_since(philo->last_eat_at, time_to_eat_ms + time_to_sleep_ms);
	return (error);
}

// If N is odd, N = 2k + 1, initial usleep should be (k * id) / N
// If N is even, N = 2k, initial usleep should be (k * id) / N
static int	philo_think(t_philo *philo)
{
	int			error;
	const int	time_to_eat_ms = philo->e->args.time_to_eat_ms;
	const int	n = philo->e->args.num_philo;
	const int	k = philo->e->args.num_philo / 2;
	const int	initial_slot = (k * philo->id) % n;

	philo->state = PH_THINKING;
	error = unsafe_log_action(philo, "is thinking", NULL);
	if (error == 0)
	{
		if (philo->eat_count == 0)
			msleep_since(philo->last_eat_at, time_to_eat_ms * initial_slot / k);
		else
			msleep_since(philo->last_eat_at, time_to_eat_ms * n / k);
	}
	return (error);
}
