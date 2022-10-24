/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 18:34:17 by susami            #+#    #+#             */
/*   Updated: 2022/10/23 22:23:53 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_H
# define UTIL_H

# include "philo.h"

// time.c
int			get_timestamp_ms(t_timeval since);
void		gettimeofday_rounddown_ms(t_timeval *tp);
t_timeval	timeadd_msec(t_timeval t, int interval_msec);
suseconds_t	timediff_usec(t_timeval start, t_timeval end);
int			timediff_msec(t_timeval start, t_timeval end);

// sleep.c
void		usleep_until(t_timeval t);
void		msleep_since(t_timeval since, int milliseconds);

// err_exit.c
void		err_exit(char *msg);
void		sem_wait_exit_on_err(sem_t *sem);
void		sem_post_exit_on_err(sem_t *sem);
#endif
