/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 02:20:50 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 23:30:43 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"
#include <stdlib.h>

int	init_status(t_table *table)
{
	table->status = ft_calloc(1, sizeof(t_status));
	if (!table->status)
		return (0);
	table->status->reason = STOP_NONE;
	if (pthread_mutex_init(&table->status->lock, NULL) != 0)
	{
		free(table->status);
		table->status = NULL;
		return (0);
	}
	return (1);
}

t_table	*init_table(int argc, char *argv[])
{
	t_table	*table;

	table = ft_calloc(1, sizeof(t_table));
	if (!table)
		return (NULL);
	if (!init_status(table))
		return (free_table(table), NULL);
	table->args = clean_args(argc, argv);
	if (!table->args)
		return (free_table(table), NULL);
	if (!create_team(table))
		return (free_table(table), NULL);
	if (!create_monitor(table))
	{
		request_stop(table, STOP_FATAL, -1);
		wait_team(table->team);
		return (free_table(table), NULL);
	}
	return (table);
}
