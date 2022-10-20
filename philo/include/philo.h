/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 19:03:03 by susami            #+#    #+#             */
/*   Updated: 2022/10/20 21:59:59 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>
# ifndef MAX_PHILO
#  define MAX_PHILO 200
# endif

typedef struct timeval		t_timeval;
typedef struct s_fork		t_fork;
typedef struct s_philo		t_philo;
typedef struct s_monitor	t_monitor;
typedef struct s_args		t_args;
typedef struct s_env		t_env;

/* ************************************************************************** */
/*                                                                            */
/*                            Function Prototypes                             */
/*                                                                            */
/* ************************************************************************** */

// philosopher.c
void	*philosopher_func(void *arg);

// unsafe_philosopher.c
bool	unsafe_is_hungry(t_philo *philo);
bool	unsafe_is_dead(t_philo *philo, t_timeval *tp);
int		unsafe_log_action(t_philo *philo, const char *msg, t_timeval *tp);
void	unsafe_log_dead(t_philo *philo, t_timeval t);

// init.c
void	init_env(t_env *e);

// monitor.c
void	*monitor_func(void *arg);

// argparse.c
void	usage_err(void) __attribute__((noreturn));
void	argparse(t_args *args, int argc, char *argv[]);

// simulation.c
void	wait_simulation_ends(t_env *e);
void	start_simulation(t_env *e);

// main.c
void	err_exit(char *msg) __attribute__((noreturn));

/* ************************************************************************** */
/*                                                                            */
/*                            Type Definitions                                */
/*                                                                            */
/* ************************************************************************** */

struct s_fork {
	pthread_mutex_t	mtx;
	int				id;
};

enum e_pstate {
	PH_EATING,
	PH_SLEEPING,
	PH_THINKING,
};

/*
	- eat_count    : will be read from monitor thread (need mutex)
	- last_eat_at  : will be read from monitor thread (need mutex)
	- state        : philo thread's private var
*/
struct s_philo {
	pthread_t		tid;
	pthread_mutex_t	mtx;
	int				id;

	int				eat_count;
	t_timeval		last_eat_at;
	enum e_pstate	state;

	t_fork			*low;
	t_fork			*high;
	t_env			*e;
};

struct s_monitor {
	pthread_t		tid;
	pthread_mutex_t	mtx;
	bool			is_dead;
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
	t_fork		forks[MAX_PHILO];
	t_args		args;
	t_timeval	started_at;
};

#endif
