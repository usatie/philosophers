/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 18:16:33 by susami            #+#    #+#             */
/*   Updated: 2022/10/19 15:13:21 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "philo.h"

// TODO: pthread_xxxのerror handling

static size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

void	err_exit(char *msg)
{
	write(2, msg, ft_strlen(msg));
	exit(1);
}

int	main(int argc, char *argv[])
{
	t_env	e;

	argparse(&e.args, argc, argv);
	init_env(&e);
	start_simulation(&e);
	wait_simulation_ends(&e);
	exit(0);
}
