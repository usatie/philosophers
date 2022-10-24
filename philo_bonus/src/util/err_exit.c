/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 15:54:14 by susami            #+#    #+#             */
/*   Updated: 2022/10/24 22:28:53 by susami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h> // STDERR_FILENO
#include <stdlib.h>
#include <semaphore.h>
#include "libftsubset.h"
#include "util.h"

void	err_exit(char *msg)
{
	ft_putstr_fd("[err_exit] ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	exit(EXIT_FAILURE);
}

sem_t	*sem_open_exit_on_err(const char *name, int value)
{
	const mode_t	perms = 0666;
	sem_t			*sem;

	sem = sem_open(name, O_CREAT, perms, value);
	if (sem == SEM_FAILED)
		err_exit("sem_open()");
	sem_unlink(name);
	return (sem);
}

void	sem_wait_exit_on_err(sem_t *sem)
{
	if (sem_wait(sem) < 0)
		err_exit("sem_wait()");
}

void	sem_post_exit_on_err(sem_t *sem)
{
	if (sem_post(sem) < 0)
		err_exit("sem_post()");
}
