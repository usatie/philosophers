/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 19:03:03 by susami            #+#    #+#             */
/*   Updated: 2022/10/20 18:20:17 by susami           ###   ########.fr       */
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

// time.c
int			get_timestamp_ms(t_timeval since);
void		gettimeofday_rounddown_ms(t_timeval *tp);
t_timeval	timeadd_msec(t_timeval t, int interval_msec);
suseconds_t	timediff_usec(t_timeval start, t_timeval end);
int			timediff_msec(t_timeval start, t_timeval end);

// sleep.c
void		usleep_until(t_timeval t);
void		msleep_since(t_timeval since, int milliseconds);

// log.c
int			philo_log(t_philo *philo, const char *msg, t_timeval *tp);
void		philo_log_died_no_monitor_lock(t_philo *philo, t_timeval t);
void		philo_log_died(t_philo *philo, t_timeval t);

// philosopher.c
bool		is_hungry(t_philo *philo);
void		*philosopher_func(void *arg);

// init.c
void		init_env(t_env *e);

// monitor.c
bool		is_dead_no_philo_lock(t_philo *philo, t_timeval *tp);
void		*monitor_func(void *arg);

// argparse.c
void		usage_err(void) __attribute__((noreturn));
void		argparse(t_args *args, int argc, char *argv[]);

// simulation.c
void		wait_simulation_ends(t_env *e);
void		start_simulation(t_env *e);

// main.c
void		err_exit(char *msg) __attribute__((noreturn));

struct s_fork {
	pthread_mutex_t	mtx;
	int				id;
};

enum e_pstate {
	PH_EATING,
	PH_SLEEPING,
	PH_THINKING,
};

struct s_philo {
	pthread_t		tid;
	pthread_mutex_t	mtx;
	int				id;
	t_env			*e;
	t_fork			*low;
	t_fork			*high;
	int				eat_count;
	t_timeval		last_eat_at;
	enum e_pstate	state;
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
