/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_access.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 02:16:53 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 11:08:07 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"
#include <unistd.h>

int	queue_dongle(t_dongle *dongle, t_coder *coder, t_waiter *waiter)
{
	long	priority;

	priority = compute_priority(dongle, coder);
	if (!init_waiter(waiter, coder, priority))
		return (request_stop(coder->table, STOP_FATAL, coder->id), 0);
	if (!heap_push(dongle->waitlist, waiter, waiter_cmp))
	{
		pthread_cond_destroy(&waiter->cond);
		return (request_stop(coder->table, STOP_FATAL, coder->id), 0);
	}
	return (1);
}

void	wait_cooldown(t_dongle *dongle, t_table *table)
{
	long	remaining;
	long	slice;

	remaining = (table->args->dongle_cooldown
			- (get_time_ms() - dongle->released));
	while (remaining > 0 && is_running(table))
	{
		if (remaining < 10)
			slice = remaining;
		else
			slice = 10;
		usleep(slice * 1000);
		remaining -= slice;
	}
}

int	try_fast_dongle(t_dongle *dongle, t_coder *coder)
{
	if (dongle->in_use)
		return (0);
	wait_cooldown(dongle, coder->table);
	dongle->in_use = 1;
	return (1);
}

int	take_dongle(t_dongle *dongle, t_coder *coder)
{
	t_waiter	waiter;

	pthread_mutex_lock(&dongle->lock);
	if (try_fast_dongle(dongle, coder))
	{
		pthread_mutex_unlock(&dongle->lock);
		return (1);
	}
	if (!queue_dongle(dongle, coder, &waiter))
	{
		pthread_mutex_unlock(&dongle->lock);
		return (0);
	}
	while (waiter.chosen == 0)
		pthread_cond_wait(&waiter.cond, &dongle->lock);
	pthread_cond_destroy(&waiter.cond);
	dongle->in_use = 1;
	pthread_mutex_unlock(&dongle->lock);
	return (1);
}

void	release_dongle(t_dongle *dongle)
{
	t_waiter	*next;

	pthread_mutex_lock(&dongle->lock);
	dongle->in_use = 0;
	dongle->released = get_time_ms();
	if (dongle->waitlist->size > 0)
	{
		next = heap_pop(dongle->waitlist, waiter_cmp);
		next->chosen = 1;
		pthread_cond_signal(&next->cond);
	}
	pthread_mutex_unlock(&dongle->lock);
}
