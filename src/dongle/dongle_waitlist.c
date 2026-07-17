/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_waitlist.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 19:14:59 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 01:22:05 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

int	take_dongle(t_dongle *dongle, t_coder *coder)
{
	t_waiter	waiter;
	long		priority;

	pthread_mutex_lock(&dongle->lock);
	// add part to check cooldown and dongle availability
	priority = compute_priority(dongle, coder);
	if (!init_waiter(&waiter, coder, priority))
	{
		pthread_mutex_unlock(&dongle->lock);
		request_stop(coder->table, STOP_FATAL, coder->id);
		return (0);
	}
	if (!heap_push(dongle->waitlist, &waiter, waiter_cmp))
	{
		pthread_cond_destroy(&waiter.cond);
		pthread_mutex_unlock(&dongle->lock);
		request_stop(coder->table, STOP_FATAL, coder->id);
		return (0);
	}
	while (waiter.chosen == 0)
		pthread_cond_wait(&waiter.cond, &dongle->lock);
	pthread_cond_destroy(&waiter.cond);
	// wait for cooldown
	pthread_mutex_unlock(&dongle->lock);
	return (1);
}

