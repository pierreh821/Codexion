/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 17:36:32 by phenry            #+#    #+#             */
/*   Updated: 2026/07/12 22:40:49 by phenry           ###   ########.fr       */
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

void	take_dongles(t_coder *coder)
{
	pthread_mutex_lock(&coder->first->lock);
	printf("%ld %d has taken a dongle\n",
		coder->table->monitor->elapsed(coder->table->monitor), coder->id);
	pthread_mutex_lock(&coder->second->lock);
	printf("%ld %d has taken a dongle\n",
		coder->table->monitor->elapsed(coder->table->monitor), coder->id);
}

void	dongle_order(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		coder->first = coder->left_dongle;
		coder->second = coder->right_dongle;
	}
	else
	{
		coder->first = coder->right_dongle;
		coder->second = coder->left_dongle;
	}
}

void	compile(t_coder *coder)
{
	coder->state = COMPILING;
	printf("%ld %d is compiling\n",
		coder->table->monitor->elapsed(coder->table->monitor), coder->id);
	usleep(coder->table->args->time_to_compile);
	pthread_mutex_unlock(&coder->second->lock);
	pthread_mutex_unlock(&coder->first->lock);
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
	int		compiles;
	int		needed;

	coder = (t_coder *)inp;
	dongle_order(coder);
	compiles = 0;
	needed = coder->table->args->number_of_compiles_required;
	wait_for_start(coder);
	usleep(coder->id % 2 * (coder->table->args->time_to_compile / 2));
	while (*(coder->run_signal) && compiles < needed)
	{
		gettimeofday(&coder->start, NULL);
		take_dongles(coder);
		compile(coder);
		debug(coder);
		refactor(coder);
		compiles++;
	}
	return (NULL);
}
