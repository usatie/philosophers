/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 09:54:12 by susami            #+#    #+#             */
/*   Updated: 2022/10/20 22:46:30 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libftsubset.h"

// s: not protected (difference between "" and NULL is important)
void	ft_putstr_fd(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
}
