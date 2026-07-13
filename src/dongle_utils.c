/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 18:18:49 by phenry            #+#    #+#             */
/*   Updated: 2026/07/13 19:39:35 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

t_dongle	*create_dongle(int id)
{
	t_dongle	*dongle;

	dongle = malloc(sizeof(t_dongle));
	if (pthread_mutex_init(&(dongle->lock), NULL) != 0)
		error("Failed to create dongle's mutex");
	dongle->id = id;
	return (dongle);
}

void	free_dongles(t_dongle **dongle_set, int nb)
{
	int	i;

	i = 0;
	while (i < nb)
	{
		pthread_mutex_destroy(&(dongle_set[i]->lock));
		free(dongle_set[i]);
	}
	free(dongle_set);
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
