/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 11:02:53 by susami            #+#    #+#             */
/*   Updated: 2022/10/20 16:44:32 by susami           ###   ########.fr       */
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

static int	pickup_forks(t_philo *philo)
{
	int	error;

	pthread_mutex_lock(&philo->low->mtx);
	error = philo_log(philo, "has taken a fork", NULL);
	if (philo->low->id == philo->high->id)
		error = -1;
	if (error == 0)
	{
		pthread_mutex_lock(&philo->high->mtx);
		error = philo_log(philo, "has taken a fork", NULL);
		if (error)
			pthread_mutex_unlock(&philo->high->mtx);
	}
	if (error)
		pthread_mutex_unlock(&philo->low->mtx);
	return (error);
}

static void	putdown_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->high->mtx);
	pthread_mutex_unlock(&philo->low->mtx);
}

int	philo_eat(t_philo *philo)
{
	int	error;

	error = pickup_forks(philo);
	if (error == 0)
	{
		pthread_mutex_lock(&philo->mtx);
		philo->eat_count++;
		philo->state = PH_EATING;
		error = philo_log(philo, "is eating", &philo->last_eat_at);
		pthread_mutex_unlock(&philo->mtx);
		if (error == 0)
			msleep_since(philo->last_eat_at, philo->e->args.time_to_eat_ms);
		putdown_forks(philo);
	}
	return (error);
}

// last_eat_atはphiloのスレッド以外からは更新されないので
// readだけならmutexの必要なし
int	philo_sleep(t_philo *philo)
{
	const int	time_to_eat_ms = philo->e->args.time_to_eat_ms;
	const int	time_to_sleep_ms = philo->e->args.time_to_sleep_ms;

	philo->state = PH_SLEEPING;
	if (philo_log(philo, "is sleeping", NULL) < 0)
		return (-1);
	msleep_since(philo->last_eat_at, time_to_eat_ms + time_to_sleep_ms);
	return (0);
}

// If N is odd, N = 2k + 1, initial usleep should be (k * id) / N
// If N is even, N = 2k, initial usleep should be (k * id) / N
int	philo_think(t_philo *philo)
{
	int			error;
	const int	time_to_eat_ms = philo->e->args.time_to_eat_ms;
	const int	n = philo->e->args.num_philo;
	const int	k = philo->e->args.num_philo / 2;
	const int	initial_slot = (k * philo->id) % n;

	error = philo_log(philo, "is thinking", NULL);
	if (error == 0)
	{
		philo->state = PH_THINKING;
		if (philo->eat_count == 0)
			msleep_since(philo->last_eat_at, time_to_eat_ms * initial_slot / k);
		else
			msleep_since(philo->last_eat_at, time_to_eat_ms * n / k);
	}
	return (error);
}

bool	is_hungry(t_philo *philo)
{
	const t_args	*args = &philo->e->args;

	if (args->max_eat < 0)
		return (true);
	return (philo->eat_count < args->max_eat);
}
