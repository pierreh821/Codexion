/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 18:15:07 by phenry            #+#    #+#             */
/*   Updated: 2026/07/14 14:43:27 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

t_table	*init_table(int argc, char *argv[], void *(*work)(void *))
{
	t_table	*table;

	table = malloc(sizeof(t_table));
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
	free_team(table->team);
	end_wait_monitor(table->monitor);
	free(table->args);
	free(table);
}

/**
 * - to get the table: pass NULL as parameter
 * - to store the table: pass the table as parameter
 * - returns the table if stord, else NULL pointer
 */
t_table	*get_table(t_table *set_table)
{
	static t_table	*stored_table = NULL;

	if (set_table != NULL)
		stored_table = set_table;
	return stored_table;
}
