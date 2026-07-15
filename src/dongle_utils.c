/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 18:18:49 by phenry            #+#    #+#             */
/*   Updated: 2026/07/15 10:51:34 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

t_dongle	*create_dongle(int id)
{
	t_dongle	*dongle;

	dongle = ft_calloc(1, sizeof(t_dongle));
    if (!dongle)
        return (NULL);
    if (pthread_mutex_init(&(dongle->lock), NULL) != 0)
    {
        free(dongle);
        return (NULL);
    }
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
		if (!team->dongle_set[id])
		{
			free_dongles(team->dongle_set, id);
			error("Failed to allocate memory for dongles");
		}
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
