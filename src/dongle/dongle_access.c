/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_access.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 02:16:53 by phenry            #+#    #+#             */
/*   Updated: 2026/08/05 03:13:39 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"
#include <stdio.h>

int	queue_dongle(t_dongle *dongle, t_coder *coder, t_waiter *waiter)
{
	long	priority;
	int		ok;

	printf("\e[0;32m  %d queuing for D%d\e[0m\n", coder->id, dongle->id);
	priority = compute_priority(dongle, coder);
	if (!init_waiter(waiter, coder, priority))
		return (request_stop(coder->table, STOP_FATAL, coder->id), 0);
	pthread_mutex_lock(&dongle->lock);
	ok = heap_push(dongle->waitlist, waiter, waiter_cmp);
	printf("\e[0;35mWAITLIST'S SIZE: %d\e[0m\n", dongle->waitlist->size);
	pthread_mutex_unlock(&dongle->lock);
	if (!ok)
	{
		pthread_cond_destroy(&waiter->cond);
		return (request_stop(coder->table, STOP_FATAL, coder->id), 0);
	}
	return (1);
}

int	take_dongle(t_dongle *dongle, t_coder *coder)
{
	t_waiter	waiter;

	if (!queue_dongle(dongle, coder, &waiter))
		return (pthread_mutex_unlock(&dongle->lock), 0);
	printf("\e[0;32m  %d trying to grant D%d\e[0m\n",
		coder->id, dongle->id);
	try_grant(dongle);
	pthread_mutex_lock(&dongle->lock);
	while (waiter.chosen == 0 && is_running(coder->table))
		pthread_cond_wait(&waiter.cond, &dongle->lock);
	if (waiter.chosen == 0)
		heap_remove_waiter(dongle->waitlist, &waiter, waiter_cmp);
	pthread_mutex_unlock(&dongle->lock);
	pthread_cond_destroy(&waiter.cond);
	return (waiter.chosen != 0);
}

void	try_grant(t_dongle *dongle)
{
	t_waiter	*top;
	long		remaining;

	pthread_mutex_lock(&dongle->lock);
	printf("\e[0;35mWAITLIST'S SIZE: %d\e[0m\n", dongle->waitlist->size);
	if (!dongle->in_use && dongle->waitlist->size > 0)
	{
		remaining = dongle->table->args->dongle_cooldown
			- (get_time_ms() - dongle->released);
		if (remaining > 0)
			sliced_sleep(dongle->table, remaining * 1000);
		top = heap_pop(dongle->waitlist, waiter_cmp);
		top->chosen = 1;
		dongle->in_use = 1;
		pthread_cond_signal(&top->cond);
		printf("\e[0;32m  %d got D%d\e[0m\n", top->coder->id, dongle->id);
	}
	printf("\e[0;35mWAITLIST'S SIZE: %d\e[0m\n", dongle->waitlist->size);
	pthread_mutex_unlock(&dongle->lock);
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
		pthread_cond_broadcast(&next->cond);
	}
	pthread_mutex_unlock(&dongle->lock);
	try_grant(dongle);
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
