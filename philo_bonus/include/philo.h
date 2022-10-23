/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 19:03:03 by susami            #+#    #+#             */
/*   Updated: 2022/10/23 09:05:00 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <semaphore.h>
# include <stdbool.h>
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

/*
	- eat_count      : will be read from monitor thread (need mutex)
	- last_eat_at    : will be read from monitor thread (need mutex)
	- last_sleep_at  : philo thread's private var
	- next_eat_at    : philo thread's private var
	- state          : philo thread's private var
*/
struct s_philo {
	int				id;
	pid_t			pid;
	sem_t			fork;

	int				eat_count;
	t_timeval		last_eat_at;
	t_timeval		last_sleep_at;
	t_timeval		next_eat_at;
	enum e_pstate	state;

	t_env			*e;
};

struct s_monitor {
	pid_t	pid;
	sem_t	*sem;
	bool	is_dead;
};

struct s_args {
	int	num_philo;
	int	time_to_die_ms;
	int	time_to_eat_ms;
	int	time_to_sleep_ms;
	int	max_eat;
};

struct s_env {
	t_monitor	monitor;
	t_philo		philosophers[MAX_PHILO];
	t_args		args;
	t_timeval	started_at;
	int			optimal_interval_ms;
};

#endif
