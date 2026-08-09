/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_schedule.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 02:18:22 by phenry            #+#    #+#             */
/*   Updated: 2026/08/05 18:44:59 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

long	compute_priority(t_dongle *dongle, t_coder *coder)
{
	long	priority;

	if (coder->table->args->strategy == FIFO)
		return (dongle->next_ticket++);
	pthread_mutex_lock(&coder->task_lock);
	priority = coder->last_compile_start + coder->table->args->time_to_burnout;
	pthread_mutex_unlock(&coder->task_lock);
	return (priority);
}

int	waiter_cmp(t_waiter *a, t_waiter *b)
{
	if (a->priority != b->priority)
		return (a->priority < b->priority);
	return (a->coder->id < b->coder->id);
}

void	wait_cooldown(t_dongle *dongle, t_coder *coder)
{
	long	remaining;
	long	released_at;
	pthread_mutex_lock(&dongle->lock);
	released_at = dongle->released;
	pthread_mutex_unlock(&dongle->lock);
	if (released_at == 0)
		return ;
	remaining = coder->table->args->dongle_cooldown
		- (get_time_ms() - released_at);
	if (remaining > 0)
		sliced_sleep(coder->table, remaining * 1000);
}
