/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 18:15:07 by phenry            #+#    #+#             */
/*   Updated: 2026/07/15 10:51:56 by phenry           ###   ########.fr       */
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
	table->args = clean_args(argc, argv);
	create_team(table, work);
	create_monitor(table);
	return (table);
}

void	free_table(t_table *table)
{
	if (table->args)
		free(table->args);
	if (table->team)
		free_team(table->team);
	if (table->monitor)
		end_wait_monitor(table->monitor);
	if (table)
		free(table);
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
