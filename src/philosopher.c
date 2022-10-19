/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 11:02:53 by susami            #+#    #+#             */
/*   Updated: 2022/10/19 21:23:52 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philosopher_func(void *arg)
{
	int		error;
	t_philo	*philo;

	error = 0;
	philo = (t_philo *)arg;
	usleep_until(philo->e->started_at);
	while (error == 0 && is_hungry(philo))
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

int	philo_eat(t_philo *philo)
{
	int	error;

	error = 0;
	pthread_mutex_lock(&philo->left->mtx);
	if (philo_log(philo, "has taken a fork") < 0)
		error = -1;
	pthread_mutex_lock(&philo->right->mtx);
	if (philo_log(philo, "has taken a fork") < 0 && error == 0)
		error = -2;
	pthread_mutex_lock(&philo->mtx);
	if (is_philo_died(philo, &philo->last_eat_at))
		error = -3;
	else
	{
		philo->eat_count++;
		philo->state = PH_EATING;
		if (philo_log(philo, "is eating") < 0 && error == 0)
			error = -4;
	}
	pthread_mutex_unlock(&philo->mtx);
	msleep_since(philo->last_eat_at, philo->e->args.time_to_eat_ms);
	pthread_mutex_unlock(&philo->left->mtx);
	pthread_mutex_unlock(&philo->right->mtx);
	return (error);
}

// last_eat_atはphiloのスレッド以外からは更新されないので
// readだけならmutexの必要なし
int	philo_sleep(t_philo *philo)
{
	const int	time_to_eat_ms = philo->e->args.time_to_eat_ms;
	const int	time_to_sleep_ms = philo->e->args.time_to_sleep_ms;

	philo->state = PH_SLEEPING;
	if (philo_log(philo, "is sleeping") < 0)
		return (-1);
	msleep_since(philo->last_eat_at, time_to_eat_ms + time_to_sleep_ms);
	return (0);
}

// If N is odd, N = 2k + 1, initial usleep should be (k * id) / N
// If N is even, N = 2k, initial usleep should be (k * id) / N
int	philo_think(t_philo *philo)
{
	const int	time_to_eat_ms = philo->e->args.time_to_eat_ms;
	const int	n = philo->e->args.num_philo;
	const int	k = philo->e->args.num_philo / 2;
	const int	initial_slot = (k * philo->id) % n;

	if (philo_log(philo, "is thinking") < 0)
		return (-1);
	philo->state = PH_THINKING;
	if (philo->eat_count == 0)
	{
		msleep_since(philo->last_eat_at, time_to_eat_ms * initial_slot / k);
	}
	else
		msleep_since(philo->last_eat_at, time_to_eat_ms * n / k);
	return (0);
}

bool	is_hungry(t_philo *philo)
{
	const t_args	*args = &philo->e->args;

	if (args->max_eat < 0)
		return (true);
	return (philo->eat_count < args->max_eat);
}
