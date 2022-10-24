/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 08:24:13 by susami            #+#    #+#             */
/*   Updated: 2022/10/24 23:21:33 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "argparse.h"
#include "philo.h"
#include "util.h"
#include "simulation.h"

int	main(int argc, char *argv[])
{
	t_env	e;

	setvbuf(stdout, NULL, _IONBF, 0);
	if (argparse(&e.args, argc, argv) == ARGPARSE_ERROR)
		err_exit("argparse()");
	init_env(&e);
	start_simulation(&e);
	wait_simulation_ends(&e);
	return (EXIT_SUCCESS);
}
