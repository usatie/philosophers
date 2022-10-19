/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 19:03:03 by susami            #+#    #+#             */
/*   Updated: 2022/10/19 11:12:26 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>
# define MAX_PHILO 200

typedef struct timeval		t_timeval;
typedef struct s_fork		t_fork;
typedef struct s_philo		t_philo;
typedef struct s_monitor	t_monitor;
typedef struct s_args		t_args;
typedef struct s_env		t_env;

int			get_timestamp_ms(t_timeval since);
void		gettimeofday_rounddown_ms(t_timeval *t);
t_timeval	timeadd_msec(t_timeval t, int interval_msec);
suseconds_t	timediff_usec(t_timeval start, t_timeval end);
void		usleep_until(t_timeval t);
void		msleep_since(t_timeval since, int milliseconds);
int			philo_log(t_philo *philo, char *msg);
int			philo_eat(t_philo *philo);
int			philo_sleep(t_philo *philo);
int			philo_think(t_philo *philo);
bool		is_hungry(t_philo *philo);
void		err_exit(char *msg) __attribute__((noreturn));
void		*philosopher_func(void *arg);
void		init_forks(t_env *e);
void		init_philosophers(t_env *e);
void		init_env(t_env *e);
void		create_philosophers_threads(t_env *e);
void		*monitor_func(void *arg);
void		create_monitor_thread(t_env *e);
void		cleanup_mutex(t_env *e);
void		argparse(t_args *args, int argc, char *argv[]);
void		wait_simulation_ends(t_env *e);
void		start_simulation(t_env *e);

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
	t_fork			*left;
	t_fork			*right;
	int				eat_count;
	t_timeval		last_eat_at;
	enum e_pstate	state;
};

struct s_monitor {
	pthread_t		tid;
	pthread_mutex_t	mtx;
	bool			is_died;
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
