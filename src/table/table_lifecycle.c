/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table_lifecycle.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 02:21:25 by phenry            #+#    #+#             */
/*   Updated: 2026/08/03 01:01:36 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"
#include <stdlib.h>
#include <unistd.h>

void	free_table(t_table *table)
{
	if (!table)
		return ;
	if (table->args)
		free(table->args);
	if (table->team)
		free_team(table->team);
	if (table->monitor)
		free_monitor(table->monitor);
	if (table->status)
	{
		pthread_mutex_destroy(&table->status->lock);
		free(table->status);
	}
	free(table);
}

void	join_table(t_table *table)
{
	wait_team(table->team);
	request_stop(table, STOP_COMPLETED, -1);
	wait_monitor(table->monitor);
}

t_table	*request_stop(t_table *table, t_stop_reason reason, int coder_id)
{
	int	first;

	pthread_mutex_lock(&table->status->lock);
	first = (table->status->reason == STOP_NONE);
	if (first)
	{
		table->status->reason = reason;
		table->status->coder_id = coder_id;
		if (table->monitor)
		{
			table->monitor->run = 0;
			if (table->monitor->logger)
				pthread_cond_broadcast(&table->monitor->logger->has_log);
		}
	}
	pthread_mutex_unlock(&table->status->lock);
	if (first && table->team)
		wake_all_waiters(table);
	return (table);
}

int	sliced_sleep(t_table *table, long time_in_us)
{
	long	start;

	start = get_time_us();
	while (is_running(table))
	{
		if (get_time_us() - start >= time_in_us)
			break ;
		usleep(500);
	}
	return (is_running(table));
}
