/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 08:24:13 by susami            #+#    #+#             */
/*   Updated: 2022/10/23 16:01:27 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "simulation.h"
#include "argparse.h"
#include "philo.h"
#include "util.h"

int	main(int argc, char *argv[])
{
	t_env	e;

	if (argparse(&e.args, argc, argv) == ARGPARSE_ERROR)
		err_exit("argparse()");
	init_env(&e);
	start_simulation(&e);
	wait_simulation_ends(&e);
	return (EXIT_SUCCESS);
}
