/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_work.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 17:36:32 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 00:06:44 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	compile(t_coder *coder)
{
	coder->start = get_time_ms();
	set_task(coder, COMPILING, 1);
	logger_write(coder, "is compiling");
	usleep(coder->table->args->time_to_compile);
	// pthread_mutex_unlock(&coder->second->); // need update soon
	// pthread_mutex_unlock(&coder->first->waitlist_lock);
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

int	work_cycle(t_coder *coder)
{
	if (!check_running_coder(coder))
		return (0);
	// take_dongles(coder);
	if (!check_running_coder(coder))
	{
		// pthread_mutex_unlock(&coder->second->waitlist_lock);
		// pthread_mutex_unlock(&coder->first->waitlist_lock);
		return (0);
	}
	compile(coder);
	debug(coder);
	refactor(coder);
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
	while (compiles < needed && work_cycle(coder))
		compiles++;
	set_task(coder, SUSPEND, 0);
	return (NULL);
}
