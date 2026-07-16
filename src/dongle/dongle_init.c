/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 18:18:49 by phenry            #+#    #+#             */
/*   Updated: 2026/07/16 20:55:38 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

t_dongle	*create_dongle(t_table *table, int id)
{
	t_dongle	*dongle;

	dongle = ft_calloc(1, sizeof(t_dongle));
	if (!dongle)
		return (NULL);
	dongle->waitlist = NULL;
	dongle->waitlist_sz = 0;
	if (pthread_mutex_init(&(dongle->waitlist_lock), NULL) != 0)
		return (free(dongle), NULL);
	if (pthread_create(&dongle->scheduler_id, NULL, schedule, dongle) != 0)
	{
		pthread_mutex_destroy(&dongle->waitlist_lock);
		free(dongle);
		return (NULL);
	}
	dongle->id = id;
	dongle->table = table;
	return (dongle);
}

void	free_dongles(t_dongle **dongle_set, int nb)
{
	int	i;

	i = 0;
	while (i < nb)
	{
		pthread_mutex_destroy(&(dongle_set[i]->waitlist_lock));
		free(dongle_set[i]->waitlist);
		free(dongle_set[i]);
		i++;
	}
	free(dongle_set);
}

int	assign_dongles(t_table *table)
{
	int		id;
	int		nb;
	t_team	*team;

	team = table->team;
	nb = team->nb;
	id = 0;
	while (id < nb)
	{
		team->dongle_set[id] = create_dongle(table, id);
		if (!team->dongle_set[id])
			return (1);
		id++;
	}
	id = 0;
	while (id < nb)
	{
		team->coders_list[id]->left_dongle = team->dongle_set[id];
		team->coders_list[id]->right_dongle = team->dongle_set[(id + 1) % nb];
		id++;
	}
	return (0);
}
