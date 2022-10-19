/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftsubset.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 16:26:27 by susami            #+#    #+#             */
/*   Updated: 2022/10/19 16:29:20 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTSUBSET_H
# define LIBFTSUBSET_H

# include <stddef.h>

int		ft_isdigit(int c);
int		ft_isspace(char c);
size_t	ft_strlen(const char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
long	ft_strtol(const char *str, char **endptr, int base);
#endif
