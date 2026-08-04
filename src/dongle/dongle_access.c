/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_access.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 02:16:53 by phenry            #+#    #+#             */
/*   Updated: 2026/08/05 00:46:52 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"
#include <stdio.h>

int	queue_dongle(t_dongle *dongle, t_coder *coder, t_waiter *waiter)
{
	long	priority;

	printf("\e[0;32m%d queueing for dongle %d\e[0m\n", coder->id, dongle->id);
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

int	try_fast_dongle(t_dongle *dongle)
{
	printf("\e[0;32mx trying to get dongle %d fast\e[0m\n", dongle->id);
	if (dongle->in_use || (dongle->waitlist && dongle->waitlist->size > 0))
		return (0);
	dongle->in_use = 1;
	return (1);
}

int	take_dongle(t_dongle *dongle, t_coder *coder)
{
	t_waiter	waiter;
	int			got_dongle;

	pthread_mutex_lock(&dongle->lock);
	got_dongle = try_fast_dongle(dongle);
	if (!got_dongle)
	{
		printf("\e[0;32m%d did not have dongle %d fast\e[0m\n", coder->id, dongle->id);
		if (!queue_dongle(dongle, coder, &waiter))
			return (pthread_mutex_unlock(&dongle->lock), 0);
		while (waiter.chosen == 0 && is_running(coder->table))
			pthread_cond_wait(&waiter.cond, &dongle->lock);
		if (waiter.chosen == 0)
		{
			heap_remove_waiter(dongle->waitlist, &waiter, waiter_cmp);
			pthread_cond_destroy(&waiter.cond);
			pthread_mutex_unlock(&dongle->lock);
			return (0);
		}
		pthread_cond_destroy(&waiter.cond);
		dongle->in_use = 1;
	}
	else
		printf("\e[0;32m%d got dongle %d fast\e[0m\n", coder->id, dongle->id);
	pthread_mutex_unlock(&dongle->lock);
	wait_cooldown(dongle, coder);
	return (1);
}

void	release_dongle(t_dongle *dongle)
{
	t_waiter	*next;

	pthread_mutex_lock(&dongle->lock);
	dongle->in_use = 0;
	if (dongle->waitlist->size > 0)
	{
		next = heap_pop(dongle->waitlist, waiter_cmp);
		next->chosen = 1;
		pthread_cond_broadcast(&next->cond);
	}
	dongle->released = get_time_ms();
	pthread_mutex_unlock(&dongle->lock);
}

void	wake_all_waiters(t_table *table)
{
	t_dongle	*dongle;
	int			i;
	int			j;

	i = 0;
	while (i < table->team->nb)
	{
		dongle = table->team->dongle_set[i];
		pthread_mutex_lock(&dongle->lock);
		j = 0;
		while (j < dongle->waitlist->size)
			pthread_cond_broadcast(&dongle->waitlist->items[j++]->cond);
		pthread_mutex_unlock(&dongle->lock);
		i++;
	}
}

void	wait_cooldown(t_dongle *dongle, t_coder *coder)
{
	long	remaining;

	if (dongle->released == 0)
		return ;
	remaining = coder->table->args->dongle_cooldown
		- (get_time_ms() - dongle->released);
	if (remaining > 0)
		sliced_sleep(coder->table, remaining * 1000);
}
