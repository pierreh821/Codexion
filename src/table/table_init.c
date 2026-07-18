/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 02:20:50 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 03:14:04 by phenry           ###   ########.fr       */
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
