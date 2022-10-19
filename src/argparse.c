/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argparse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 15:13:06 by susami            #+#    #+#             */
/*   Updated: 2022/10/19 16:37:01 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include "libftsubset.h"
#include "philo.h"

static bool	is_numeric(const char *s);
static int	get_int(const char *s);

void	argparse(t_args *args, int argc, char *argv[])
{
	if (argc < 5 || argc > 6)
		usage_err();
	args->num_philo = get_int(argv[1]);
	args->time_to_die_ms = get_int(argv[2]);
	args->time_to_eat_ms = get_int(argv[3]);
	args->time_to_sleep_ms = get_int(argv[4]);
	if (argc == 6)
		args->max_eat = get_int(argv[5]);
	else
		args->max_eat = -1;
}	

void	usage_err(void)
{
	err_exit(
		"Usage: ./philo number_of_philosophers time_to_die time_to_eat "
		"time_to_sleep [number_of_times_each_philosopher_must_eat]\n"
		);
}

static int	get_int(const char *s)
{
	long	res;
	char	*endptr;

	if (!is_numeric(s))
		usage_err();
	errno = 0;
	res = ft_strtol(s, &endptr, 10);
	if (errno != 0)
		usage_err();
	if (*endptr != '\0')
		usage_err();
	if (res < 0 || res >= 360)
		usage_err();
	return ((int)res);
}

static bool	is_numeric(const char *s)
{
	if (*s == '\0')
		return (false);
	while (*s)
	{
		if (!ft_isdigit(*s))
			return (false);
		s++;
	}
	return (true);
}
