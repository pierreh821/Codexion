/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_dongles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 22:53:19 by phenry            #+#    #+#             */
/*   Updated: 2026/07/12 22:53:27 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	launch_threads(t_table *table, void *(*work)(void *))
{
	int	i;

	i = 0;
	while (i < table->team->nb)
	{
		if (pthread_create(&table->team->coders_list[i]->thread_id, NULL, work,
				table->team->coders_list[i]) != 0)
			error("Failed to create thread");
		i++;
	}
}

void	assign_dongles(t_team *team)
{
	int			id;
	int			nb;

	nb = team->nb;
	id = 0;
	while (id < nb)
	{
		team->dongle_set[id] = create_dongle(id);
		id++;
	}
	id = 0;
	while (id < nb)
	{
		team->coders_list[id]->left_dongle = team->dongle_set[id];
		team->coders_list[id]->right_dongle = team->dongle_set[(id + 1) % nb];
		id++;
	}
}
