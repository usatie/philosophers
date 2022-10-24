/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:06:19 by susami            #+#    #+#             */
/*   Updated: 2022/10/24 15:41:31 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "util.h"
#include "simulation.h"

#define ANY_CHILD_PROCESS -1
#define NO_OPTION 0

static int	philo_simulation(t_philo *philo);
static void	kill_philosophers_processes(t_env *e);

void	start_simulation(t_env *e)
{
	int		i;
	pid_t	child_pid;

	i = 0;
	while (i < e->args.num_philo)
	{
		child_pid = fork();
		if (child_pid < 0)
			err_exit("fork()");
		else if (child_pid == 0)
			exit(philo_simulation(&e->philosophers[i]));
		else
			e->philosophers[i].pid = child_pid;
		i++;
	}
}

void	wait_simulation_ends(t_env *e)
{
	int		status;
	pid_t	child_pid;

	while (true)
	{
		child_pid = waitpid(ANY_CHILD_PROCESS, &status, NO_OPTION);
		if (child_pid < 0 || status != EXIT_SUCCESS)
		{
			kill_philosophers_processes(e);
			return ;
		}
	}
}

static int	philo_simulation(t_philo *philo)
{
	if (pthread_create(&philo->tid, NULL, philosopher_func, philo) < 0)
		err_exit("pthread_create(philo)");
	if (pthread_create(&philo->monitor_tid, NULL, monitor_func, philo) < 0)
		err_exit("pthread_create(monitor)");
	if (pthread_join(philo->monitor_tid, NULL) < 0)
		err_exit("pthread_join(monitor)");
	if (pthread_join(philo->tid, NULL) < 0)
		err_exit("pthread_join(philo)");
	return (EXIT_SUCCESS);
}

static void	kill_philosophers_processes(t_env *e)
{
	int		i;

	i = 0;
	while (i < e->args.num_philo)
	{
		kill(e->philosophers[i].pid, SIGKILL);
		i++;
	}
}
