/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_action.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 18:54:38 by susami            #+#    #+#             */
/*   Updated: 2022/10/24 18:58:10 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include "simulation.h"

static void	increment_eat_count(t_philo *philo);

void	philo_eat(t_philo *philo)
{
	const int	interval_ms = philo->e->optimal_interval_ms;
	const int	time_to_eat_ms = philo->e->args.time_to_eat_ms;

	philo->state = PH_EATING;
	sem_wait_exit_on_err(philo->e->waiters);
	sem_wait_exit_on_err(philo->e->forks);
	log_action(philo, "has taken a fork", NULL, NULL);
	sem_wait_exit_on_err(philo->e->forks);
	log_action(philo, "has taken a fork", NULL, NULL);
	sem_post_exit_on_err(philo->e->waiters);
	sem_wait_exit_on_err(philo->self);
	log_action(philo, "is eating", &philo->last_eat_at, increment_eat_count);
	sem_post_exit_on_err(philo->self);
	philo->next_eat_at = timeadd_msec(philo->last_eat_at, interval_ms);
	msleep_since(philo->last_eat_at, time_to_eat_ms);
}

void	philo_sleep(t_philo *philo)
{
	const int	time_to_sleep_ms = philo->e->args.time_to_sleep_ms;

	philo->state = PH_SLEEPING;
	log_action(philo, "is sleeping", &philo->last_sleep_at, putdown_forks);
	msleep_since(philo->last_sleep_at, time_to_sleep_ms);
}

void	philo_think(t_philo *philo)
{
	philo->state = PH_THINKING;
	log_action(philo, "is thinking", NULL, NULL);
	usleep_until(philo->next_eat_at);
}

void	putdown_forks(t_philo *philo)
{
	sem_post_exit_on_err(philo->e->forks);
	sem_post_exit_on_err(philo->e->forks);
}

/* ************************************************************************** */
/*                                                                            */
/*                          File Private Functions                            */
/*                                                                            */
/* ************************************************************************** */

static void	increment_eat_count(t_philo *philo)
{
	philo->eat_count++;
}
