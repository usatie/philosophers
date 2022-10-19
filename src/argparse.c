/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argparse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 15:13:06 by susami            #+#    #+#             */
/*   Updated: 2022/10/19 15:13:20 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	argparse(t_args *args, int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	args->num_philo = 7;
	args->time_to_die_ms = 700;
	args->time_to_eat_ms = 300;
	args->time_to_sleep_ms = 100;
	args->max_eat = 10;
}
