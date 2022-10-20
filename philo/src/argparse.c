/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argparse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 15:13:06 by susami            #+#    #+#             */
/*   Updated: 2022/10/20 10:48:49 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <errno.h>
#include "libftsubset.h"
#include "philo.h"

/*
If stringizer macro is allowed by norminette, it's convenient.

#define VAL(str) #str
#define TOSTRING(str) VAL(str)

This can be used like this.
char *s = "between 2 to " TOSTRING(MAX_PHILO) ".\n";
*/

static bool	is_numeric(const char *s);
static int	get_int(const char *s);

void	argparse(t_args *args, int argc, char *argv[])
{
	if (argc < 5 || argc > 6)
		usage_err();
	args->num_philo = get_int(argv[1]);
	if (args->num_philo <= 0 || args->num_philo > MAX_PHILO)
		usage_err();
	args->time_to_die_ms = get_int(argv[2]);
	args->time_to_eat_ms = get_int(argv[3]);
	args->time_to_sleep_ms = get_int(argv[4]);
	if (argc == 6)
		args->max_eat = get_int(argv[5]);
	else
		args->max_eat = -1;
	if (args->max_eat == 0)
		usage_err();
}

void	usage_err(void)
{
	err_exit(
		"USAGE:\n"
		"  ./philo number_of_philosophers time_to_die time_to_eat "
		"time_to_sleep [number_of_times_each_philosopher_must_eat]\n"
		"\n"
		"DESCRIPTION:\n"
		"  - number_of_philosophers is valid between 2 to MAX_PHILO.\n"
		"    (Default value for MAX_PHILO is 200.)\n"
		"  - number_of_times_each_philosopher_must_eat must be non-zero\n"
		"    value. When it is negative or not specified, the simulation\n"
		"    does not end until someone dies.\n"
		"VALID EXAMPLE:\n"
		"  ./philo 5 500 100 100\n"
		"  ./philo 5 500 100 100 10\n"
		"INVALID EXAMPLE:\n"
		"  ./philo 5 500 100\n"
		"  ./philo 5 500 100 100 10 100\n"
		"  ./philo 5 9999999999999999 100 100 10\n"
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
	if (res < 0 || res > INT_MAX)
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
