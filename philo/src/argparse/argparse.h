/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argparse.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 22:35:51 by susami            #+#    #+#             */
/*   Updated: 2022/10/20 22:36:59 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGPARSE_H
# define ARGPARSE_H

# include "philo.h"

// argparse.c
void	usage_err(void) __attribute__((noreturn));
void	argparse(t_args *args, int argc, char *argv[]);

#endif
