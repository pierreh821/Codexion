/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 18:15:07 by phenry            #+#    #+#             */
/*   Updated: 2026/07/15 12:51:13 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

t_table	*init_table(int argc, char *argv[], void *(*work)(void *))
{
	t_table	*table;

	table = ft_calloc(1, sizeof(t_table));
	if (!table)
		error("Cannot allocate memory for table");
	get_table(table);
	table->status = ft_calloc(1, sizeof(t_status));
	if (!table->status)
		error("Cannot allocate memory for table status");
	table->status->reason = STOP_NONE;
	table->args = clean_args(argc, argv);
	create_team(table, work);
	create_monitor(table);
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

/**
 * - to get the table: pass NULL as parameter
 * - to store the table: pass the table as parameter
 * - to remove the table: pass (void *)-1 as parameter
 * - returns the table if stord, else NULL pointer
 */
t_table	*get_table(t_table *set_table)
{
	static t_table	*stored_table = NULL;

	if (set_table == (void *)-1)
		stored_table = NULL;
	else if (set_table != NULL)
		stored_table = set_table;
	return stored_table;
}

void	join_table(t_table *table)
{
	wait_team(table->team);
	wait_monitor(table->monitor);
}

int	request_stop(t_stop_reason reason, int	coder_id)
{
	t_table	*table;
	int		first;

	table = get_table(NULL);
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
