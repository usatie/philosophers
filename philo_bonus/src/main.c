/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 08:24:13 by susami            #+#    #+#             */
/*   Updated: 2022/10/23 08:24:30 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "argparse.h"
#include "philo.h"

// TODO: pthread_xxxのerror handling

int	main(int argc, char *argv[])
{
	t_env	e;

	if (argparse(&e.args, argc, argv) == ARGPARSE_ERROR)
		return (EXIT_FAILURE);
	//init_env(&e);
	//start_simulation(&e);
	//wait_simulation_ends(&e);
	return (EXIT_SUCCESS);
}
