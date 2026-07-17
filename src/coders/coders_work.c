/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_work.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 17:36:32 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 01:38:34 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	compile(t_coder *coder)
{
	coder->start = get_time_ms();
	set_task(coder, COMPILING, 1);
	logger_write(coder, "is compiling");
	usleep(coder->table->args->time_to_compile);
	release_dongle(coder->second);
	release_dongle(coder->first);
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

int	take_dongles(t_coder *coder)
{
	if (!take_dongle(coder->first, coder))
		return (0);
	logger_write(coder, "has taken a dongle");
	if (!take_dongle(coder->second, coder))
	{
		release_dongle(coder->first);
		return (0);
	}
	logger_write(coder, "has taken a dongle");
	return (1);
}

int	work_cycle(t_coder *coder)
{
	if (!check_running_coder(coder))
		return (0);
	if (!take_dongles(coder))
		return (0);
	if (!check_running_coder(coder))
	{
		release_dongle(coder->second);
		release_dongle(coder->first);
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
