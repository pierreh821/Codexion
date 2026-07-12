/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 17:36:32 by phenry            #+#    #+#             */
/*   Updated: 2026/07/12 18:12:22 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	wait_for_start(t_coder *coder)
{
	pthread_mutex_lock(coder->run_lock);
	while (*(coder->run_signal) == 0)
	{
		pthread_cond_wait(coder->run, coder->run_lock);
	}
	pthread_mutex_unlock(coder->run_lock);
}

void	take_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	printf("%ld %d has taken a dongle\n",
		coder->table->monitor->elapsed(coder->table->monitor), coder->id);
}

void	compile(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	coder->state = COMPILING;
	if (coder->id % 2 == 0)
	{
		first = coder->left_dongle;
		second = coder->right_dongle;
	}
	else
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	take_dongle(coder, first);
	take_dongle(coder, second);
	printf("%ld %d is compiling\n",
		coder->table->monitor->elapsed(coder->table->monitor), coder->id);
	usleep(coder->table->args->time_to_compile);
	pthread_mutex_unlock(&second->lock);
	pthread_mutex_unlock(&first->lock);
}

void	debug(t_coder *coder)
{
	coder->state = DEBUGGING;
	printf("%ld %d is debugging\n",
		coder->table->monitor->elapsed(coder->table->monitor), coder->id);
	usleep(coder->table->args->time_to_debug);
}

void	refactor(t_coder *coder)
{
	coder->state = REFACTORING;
	printf("%ld %d is refactoring\n",
		coder->table->monitor->elapsed(coder->table->monitor), coder->id);
	usleep(coder->table->args->time_to_refactor);
}

void	*work(void *inp)
{
	t_coder	*coder;

	coder = (t_coder *)inp;
	wait_for_start(coder);
	while (*(coder->run_signal))
	{
		gettimeofday(&coder->start, NULL);
		compile(coder);
		debug(coder);
		refactor(coder);
	}
	return (NULL);
}
