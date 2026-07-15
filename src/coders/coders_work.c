/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_work.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 17:36:32 by phenry            #+#    #+#             */
/*   Updated: 2026/07/15 23:07:39 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	compile(t_coder *coder)
{
	coder->start = get_time_ms();
	set_task(coder, COMPILING, 1);
	logger_write(coder, "is compiling");
	usleep(coder->table->args->time_to_compile);
	pthread_mutex_unlock(&coder->second->lock);
	pthread_mutex_unlock(&coder->first->lock);
}

void	debug(t_coder *coder)
{
	set_task(coder, DEBUGGING, 0);
	logger_write(coder, "is debugging");
	usleep(coder->table->args->time_to_debug);
}

void	refactor(t_coder *coder)
{
	set_task(coder, REFACTORING, 0);
	logger_write(coder, "is refactoring");
	usleep(coder->table->args->time_to_refactor);
}

int	check_running(t_coder *coder)
{
	pthread_mutex_lock(coder->run_lock);
	if (!*(coder->run_signal) || !coder->table->monitor->run)
	{
		pthread_mutex_unlock(coder->run_lock);
		return (0);
	}
	pthread_mutex_unlock(coder->run_lock);
	return (1);
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
	while (compiles < needed)
	{
		if (!check_running(coder))
			break;
		take_dongles(coder);
		if (!check_running(coder))
		{
			pthread_mutex_unlock(&coder->second->lock);
			pthread_mutex_unlock(&coder->first->lock);
			break;
		}
		compile(coder);
		debug(coder);
		refactor(coder);
		compiles++;
	}
	set_task(coder, SUSPEND, 0);
	return (NULL);
}
