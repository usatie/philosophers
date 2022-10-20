/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argparse.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 22:35:51 by susami            #+#    #+#             */
/*   Updated: 2022/10/20 22:54:11 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGPARSE_H
# define ARGPARSE_H

# include "philo.h"
# define ARGPARSE_ERROR -1
# define ARGPARSE_SUCCESS 0

// argparse.c
int	argparse(t_args *args, int argc, char *argv[]);

#endif
