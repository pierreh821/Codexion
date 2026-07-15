/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 21:16:51 by phenry            #+#    #+#             */
/*   Updated: 2026/07/15 13:08:37 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

int	main(int argc, char *argv[])
{
	t_table			*table;
	t_stop_reason	reason;

	table = init_table(argc, argv, &work);
	if (!table)
		return (1);
	team_start(table);
	join_table(table);
	reason = table->status->reason;
	free_table(table);
	if (reason == STOP_BURNOUT)
		return (1);
	return (0);
}
