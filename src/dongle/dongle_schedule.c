/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_schedule.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 02:18:22 by phenry            #+#    #+#             */
/*   Updated: 2026/07/19 23:47:57 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

long	compute_priority(t_dongle *dongle, t_coder *coder)
{
	long	priority;

	if (coder->table->args->strategy == FIFO)
		return (dongle->next_ticket++);
	pthread_mutex_lock(&coder->task_lock);
	priority = coder->start + coder->table->args->time_to_burnout;
	pthread_mutex_unlock(&coder->task_lock);
	return (priority);
}

int	waiter_cmp(t_waiter *a, t_waiter *b)
{
	if (a->priority != b->priority)
		return (a->priority < b->priority);
	return (a->coder->id < b->coder->id);
}
