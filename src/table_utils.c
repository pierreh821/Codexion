/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 18:15:07 by phenry            #+#    #+#             */
/*   Updated: 2026/07/12 00:26:19 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

t_table	*init_table(int argc, char *argv[], void *(*work)(void *))
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	if (!table)
		error("Cannot allocate memory for table");
	table->args = clean_args(argc, argv);
	create_monitor(table);
	create_team(table, work);
	return (table);
}

void	free_table(t_table *table)
{
	free_team(table->team);
	end_wait_monitor(table->monitor);
	free(table->args);
	free(table);
}
