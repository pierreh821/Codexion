/* ************************************************************************** */
/*		                                                                    */
/*                                                        :::      ::::::::   */
/*   coders_cycle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 02:11:05 by phenry            #+#    #+#             */
/*   Updated: 2026/08/03 01:51:47 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

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

void	wait_for_ready(t_coder *coder)
{
	long	r1;
	long	r2;
	long	max_wait;

	r1 = 0;
	r2 = 0;
	pthread_mutex_lock(&coder->first->lock);
	if (coder->first->released > 0)
		r1 = coder->table->args->dongle_cooldown - (
				get_time_ms() - coder->first->released);
	pthread_mutex_unlock(&coder->first->lock);

	pthread_mutex_lock(&coder->second->lock);
	if (coder->second->released > 0)
		r2 = coder->table->args->dongle_cooldown - (
				get_time_ms() - coder->second->released);
	pthread_mutex_unlock(&coder->second->lock);

	max_wait = r1;
	if (r2 > r1)
		max_wait = r2;
	if (max_wait > 0)
		sliced_sleep(coder->table, max_wait * 1000);
}

int	take_dongles(t_coder *coder)
{
	wait_for_ready(coder);
	if (!take_dongle(coder->first, coder))
		return (0);
	if (!take_dongle(coder->second, coder))
	{
		release_dongle(coder->first);
		return (0);
	}
	wait_cooldown(coder->first, coder);
	logger_write(coder, "has taken a dongle");
	wait_cooldown(coder->second, coder);
	logger_write(coder, "has taken a dongle");
	return (1);
}

int	work_cycle(t_coder *coder)
{
	if (!is_running(coder->table))
		return (0);
	if (!take_dongles(coder))
		return (0);
	if (!is_running(coder->table))
	{
		if (coder->second != coder->first)
			release_dongle(coder->second);
		release_dongle(coder->first);
		return (0);
	}
	compile(coder);
	debug(coder);
	refactor(coder);
	return (1);
}
