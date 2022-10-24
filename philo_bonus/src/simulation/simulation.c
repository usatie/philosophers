/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 09:21:02 by susami            #+#    #+#             */
/*   Updated: 2022/10/23 18:28:06 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "simulation.h"

static void	create_philosophers_processes(t_env *e);
static void	create_monitor_process(t_env *e);
static void	cleanup_semaphores(void);

void	start_simulation(t_env *e)
{
	create_philosophers_processes(e);
	create_monitor_process(e);
}

#include <stdio.h>
#include <signal.h>
#include <errno.h>
void	wait_simulation_ends(t_env *e)
{
	int	i;
	int	status;
	pid_t	child_id;

	// TODO: waitpid arguments
	//printf("waitpid monitor(%d)\n", e->monitor.pid);
	child_id = waitpid(e->monitor.pid, &status, 0);
	(void)child_id;
	//printf("child_id = %d, status = %d\n", child_id, status);
	i = 0;
	while (i < e->args.num_philo)
	{
		//printf("waitpid philo[%d](%d)\n", i, e->philosophers[i].pid);
		status = kill(e->philosophers[i].pid, SIGKILL);
		//child_id = waitpid(e->philosophers[i].pid, &status, 0);
		//printf("child_id = %d, status = %d\n", child_id, status);
		//printf("status = %d, errno = %d\n", status, errno);
		i++;
	}
	cleanup_semaphores();
	exit(0);
}

/* ************************************************************************** */
/*                                                                            */
/*                          File Private Functions                            */
/*                                                                            */
/* ************************************************************************** */

static void	create_philosophers_processes(t_env *e)
{
	int		i;
	pid_t	child_pid;
	t_philo	*philo;

	i = 0;
	while (i < e->args.num_philo)
	{
		philo = &e->philosophers[i];
		child_pid = fork();
		if (child_pid < 0)
			exit(EXIT_FAILURE); // error
		else if (child_pid == 0)
			exit(philosopher_func(philo)); // child
		else
		{
			// parent
			philo->pid = child_pid;
		}
		i++;
	}
}

static void	create_monitor_process(t_env *e)
{
	e->monitor.pid = fork();
	if (e->monitor.pid < 0)
		exit(EXIT_FAILURE); // error
	else if (e->monitor.pid == 0)
		exit(monitor_func(e)); // error
	else
	{
		// parent
	}
}

// TODO: What if deadlock is happening when destroying mutex?
// Deadl lock never happens with my program!
#include <string.h>
static void	cleanup_semaphores(void)
{
	int	i;

	printf("cleanup_semaphores\n");
	sem_unlink("philosophers.monitor");
	sem_unlink("philosophers.forks");
	sem_unlink("philosophers.waiters");
	i = 0;
	while (i < MAX_PHILO)
	{
		char name[128] = "philosophers.philosophers.";
		char id[3] = "";
		id[0] = i / 100;
		id[1] = (i % 100) / 10;
		id[2] = i % 10;
		strlcat(name, id, 128);
		sem_unlink(name);
		i++;
	}
}
