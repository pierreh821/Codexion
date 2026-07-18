/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 02:20:50 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 16:17:10 by phenry           ###   ########.fr       */
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
	if (!create_monitor(table))
		return (free_table(table), NULL);
	if (!create_team(table, work))
		return (free_table(table), NULL);
	return (table);
}
