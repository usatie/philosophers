/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 18:16:33 by susami            #+#    #+#             */
/*   Updated: 2022/10/19 11:12:33 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "philo.h"

// TODO: pthread_xxxのerror handling

void	err_exit(char *msg)
{
	write(2, msg, strlen(msg));
	exit(1);
}

void	argparse(t_args *args, int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	args->num_philo = 199;
	args->time_to_die_ms = 1999;
	args->time_to_eat_ms = 990;
	args->time_to_sleep_ms = 1000;
	args->max_eat = 10;
}

int	main(int argc, char *argv[])
{
	t_env	e;

	argparse(&e.args, argc, argv);
	init_env(&e);
	start_simulation(&e);
	wait_simulation_ends(&e);
	return (0);
}
