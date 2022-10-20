/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 18:16:33 by susami            #+#    #+#             */
/*   Updated: 2022/10/20 23:13:22 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "libftsubset.h"
#include "simulation.h"
#include "argparse.h"
#include "philo.h"

// TODO: pthread_xxxのerror handling

void	err_exit(char *msg)
{
	write(2, msg, ft_strlen(msg));
	exit(1);
}

int	main(int argc, char *argv[])
{
	t_env	e;

	if (argparse(&e.args, argc, argv) == ARGPARSE_ERROR)
		return (EXIT_FAILURE);
	init_env(&e);
	start_simulation(&e);
	wait_simulation_ends(&e);
	return (EXIT_SUCCESS);
}
