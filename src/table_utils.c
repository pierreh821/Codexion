/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 18:15:07 by phenry            #+#    #+#             */
/*   Updated: 2026/07/09 18:31:08 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_table	*init_table(int argc, char *argv[])
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	if (!table)
		error("Cannot allocate memory for table");
	table->monitor = create_monitor();
	table->args = clean_args(argc, argv);
	table->team = create_coders(table->args->number_of_coders, table->monitor);
	return (table);
}

void	free_table(t_table *table)
{
	free_team(table->team);
	end_wait_monitor(table->monitor);
	free(table->args);
	free(table);
}
