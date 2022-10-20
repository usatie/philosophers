/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 22:13:54 by susami            #+#    #+#             */
/*   Updated: 2022/10/20 22:15:08 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMULATION_H
# define SIMULATION_H

# include "philo.h"

// simulation.c
void	wait_simulation_ends(t_env *e);
void	start_simulation(t_env *e);

// philosopher.c
void	*philosopher_func(void *arg);

// unsafe_philosopher.c
bool	unsafe_is_hungry(t_philo *philo);
bool	unsafe_is_dead(t_philo *philo, t_timeval *tp);
int		unsafe_log_action(t_philo *philo, const char *msg, t_timeval *tp);
void	unsafe_log_dead(t_philo *philo, t_timeval t);

// monitor.c
void	*monitor_func(void *arg);
#endif
