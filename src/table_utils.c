/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 18:15:07 by phenry            #+#    #+#             */
/*   Updated: 2026/07/15 15:16:42 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

t_table	*init_table(int argc, char *argv[], void *(*work)(void *))
{
	t_table	*table;

	table = ft_calloc(1, sizeof(t_table));
	if (!table)
		return (NULL);
	table->status = ft_calloc(1, sizeof(t_status));
	if (!table->status)
		return (free_table(table), NULL);
	table->status->reason = STOP_NONE;
	table->args = clean_args(argc, argv);
	if (!table->args)
		return (free_table(table), NULL);
	create_team(table, work);
	if (!table->team)
		return (free_table(table), NULL);
	create_monitor(table);
	if (!table->monitor)
		return (free_table(table), NULL);
	return (table);
}

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
}

void	join_table(t_table *table)
{
	wait_team(table->team);
	wait_monitor(table->monitor);
}

int	request_stop(t_table *table, t_stop_reason reason, int	coder_id)
{
	int		first;

	pthread_mutex_lock(&table->status->lock);
	first = (table->status->reason == STOP_NONE);
	if (first)
	{
		table->status->reason = reason;
		table->status->coder_id = coder_id;
		table->monitor->run = 0;
	}
	pthread_mutex_unlock(&table->status->lock);
	return (first);
}
