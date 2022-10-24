/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 09:21:20 by susami            #+#    #+#             */
/*   Updated: 2022/10/24 18:57:49 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMULATION_H
# define SIMULATION_H

# include "philo.h"

typedef void	t_logfunc(t_philo *philo);

// simulation.c
void	start_simulation(t_env *e);
void	wait_simulation_ends(t_env *e);

// log.c
void	assert_alive(t_philo *philo, t_timeval *tp);
void	log_action(t_philo *philo, char *s, t_timeval *tp, t_logfunc *f);

// philosopher.c
void	*philosopher_func(void *arg);

// philosopher_action.c
void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);
void	putdown_forks(t_philo *philo);

// monitor.c
void	*monitor_func(void *arg);

#endif
