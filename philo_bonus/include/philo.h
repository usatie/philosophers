/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 19:03:03 by susami            #+#    #+#             */
/*   Updated: 2022/10/24 12:51:06 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdbool.h>
# include <sys/time.h>
# include <semaphore.h>
# include <pthread.h>
# ifndef MAX_PHILO
#  define MAX_PHILO 200
# endif

typedef struct timeval		t_timeval;
typedef struct s_philo		t_philo;
typedef struct s_monitor	t_monitor;
typedef struct s_args		t_args;
typedef struct s_env		t_env;

/* ************************************************************************** */
/*                                                                            */
/*                            Function Prototypes                             */
/*                                                                            */
/* ************************************************************************** */

// init.c
void	init_env(t_env *e);

/* ************************************************************************** */
/*                                                                            */
/*                            Type Definitions                                */
/*                                                                            */
/* ************************************************************************** */

enum e_pstate {
	PH_EATING,
	PH_SLEEPING,
	PH_THINKING,
};

struct s_philo {
	int	id;
};

struct s_args {
	int	num_philo;
	int	time_to_die_ms;
	int	time_to_eat_ms;
	int	time_to_sleep_ms;
	int	max_eat;
};

struct s_env {
	t_args		args;
	t_timeval	started_at;
	int			optimal_interval_ms;
};

#endif
