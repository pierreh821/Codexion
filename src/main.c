/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 21:16:51 by phenry            #+#    #+#             */
/*   Updated: 2026/07/12 01:14:24 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

int	main(int argc, char *argv[])
{
	t_table	*table;

	table = init_table(argc, argv, &work);
	describe_tm(table->team);
	printf("describe over, let's resume work\n");
	team_start(table);
	usleep(100000);
	team_pause(table->team);
	free_table(table);
	return (0);
}
