/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtol.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/28 23:18:22 by susami            #+#    #+#             */
/*   Updated: 2022/10/19 16:30:05 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include "libftsubset.h"

static bool	is_cut(unsigned long current, unsigned long next,
	unsigned long base)
{
	unsigned long	cutoff;
	unsigned long	cutlim;

	cutoff = LONG_MAX / base;
	cutlim = LONG_MAX % base;
	if (current > cutoff || (current == cutoff && next > cutlim))
		return (true);
	else
		return (false);
}

static void	preprocess_strtol(const char **str, int *base, bool *isneg)
{
	while (ft_isspace(**str))
		(*str)++;
	*isneg = false;
	if (**str == '-')
		*isneg = true;
	if (**str == '-' || **str == '+')
		(*str)++;
	if ((base == 0 || *base == 16)
		&& (ft_strncmp(*str, "0x", 2) == 0 || ft_strncmp(*str, "0X", 2) == 0))
	{
		*str += 2;
		*base = 16;
	}
	if (*base == 0 && **str == '0')
		*base = 8;
	else if (base == 0)
		*base = 10;
}

// RETURNS true if OVERFLOW
static bool	internal_strtol(const char **str, int base,
	unsigned long *ul_val, bool *isneg)
{
	preprocess_strtol(str, &base, isneg);
	*ul_val = 0;
	while (ft_isdigit(**str))
	{
		if (is_cut(*ul_val, (unsigned long)**str - '0', (unsigned long)base))
			return (true);
		*ul_val = (*ul_val) * (unsigned long)base
			+ (unsigned long)(**str - '0');
		(*str)++;
	}
	return (false);
}

// Original strtol takes endptr and base
// 	long strtol(const char *restrict str, char **restrict endptr, int base);
// This time, only implemented the pattern of (endptr: NULL, base: 10).
long	ft_strtol(const char *str, char **endptr, int base)
{
	unsigned long	ul_val;
	bool			isneg;
	bool			err;

	err = internal_strtol(&str, base, &ul_val, &isneg);
	if (endptr)
		*endptr = (char *)str;
	if (err)
	{
		if (isneg == true)
			ul_val = (unsigned long)LONG_MIN;
		else
			ul_val = (unsigned long)LONG_MAX;
		errno = ERANGE;
	}
	else if (isneg == true)
		ul_val = -ul_val;
	return ((long)ul_val);
}
