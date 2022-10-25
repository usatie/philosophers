/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 19:03:03 by susami            #+#    #+#             */
/*   Updated: 2022/10/25 18:57:53 by susami           ###   ########.fr       */
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

// deinit.c
void	deinit_env(t_env *e);

/* ************************************************************************** */
/*                                                                            */
/*                            Type Definitions                                */
/*                                                                            */
/* ************************************************************************** */

typedef enum e_pstate {
	PH_EATING,
	PH_SLEEPING,
	PH_THINKING,
}	t_state;

/*
	- eat_count      : will be read from monitor thread (need `self` semaphore)
	- last_eat_at    : will be read from monitor thread (need `self` semaphore)
	- last_sleep_at  : philo thread's private var
	- next_eat_at    : philo thread's private var
	- state          : philo thread's private var
*/
struct s_philo {
	int			id;
	pid_t		pid;
	pthread_t	tid;
	pthread_t	monitor_tid;

	int			eat_count;
	t_timeval	last_eat_at;
	t_timeval	last_sleep_at;
	t_timeval	next_eat_at;
	t_state		state;

	sem_t		*self;
	t_env		*e;
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
	t_philo		philosophers[MAX_PHILO];
	sem_t		*forks;
	sem_t		*waiters;
	sem_t		*log;
	sem_t		*log_dead;
};

#endif
