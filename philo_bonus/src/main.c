/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 08:24:13 by susami            #+#    #+#             */
/*   Updated: 2022/10/24 12:52:23 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "argparse.h"
#include "philo.h"
#include "util.h"

int	main(int argc, char *argv[])
{
	t_env	e;

	if (argparse(&e.args, argc, argv) == ARGPARSE_ERROR)
		err_exit("argparse()");
	init_env(&e);
	return (EXIT_SUCCESS);
}
