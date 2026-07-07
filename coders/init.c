/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 03:23:32 by phenry            #+#    #+#             */
/*   Updated: 2026/07/07 19:37:51 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/time.h>

#include <stdio.h>
#include "coders.h"
#include "codexion.h"

t_team	*alloc_coders(int nb)
{
	t_team	*team;

	team = malloc(sizeof(t_team));
	if (!team)
		error("Cannot allocate memory for team");
	team->coders_list = malloc(sizeof(t_coder *) * nb);
	if (!team->coders_list)
		error("Cannot allocate memory for coders list");
	return (team);
}

void assign_mutex(t_team *team)
{
	if (pthread_mutex_init(&(team->global_lock), NULL) != 0)
		error("Failed to init mutex");
	pthread_mutex_lock(&(team->global_lock));
}

void	assign_threads(t_team *team)
{
	int	i;

	i = 0;
	while (i < team->nb)
	{
		team->coders_list[i] = malloc(sizeof(t_coder));
		team->coders_list[i]->id = i + 1;
		team->coders_list[i]->global_lock = &(team->global_lock);
		if (pthread_create(&team->coders_list[i]->thread_id, NULL, foo,
				team->coders_list[i]) != 0)
			error("Failed to create thread");
		i++;
	}
}

void	assign_dongles(t_team *team)
{
	int			id;
	t_dongle	*new_dongle;

	id = 0;
	while (id < team->nb)
	{
		new_dongle = create_dongle(id);
		team->coders_list[id]->left_dongle = new_dongle;
		if (id > 0)
			team->coders_list[id - 1]->right_dongle = new_dongle;
		else
			team->coders_list[team->nb - 1]->right_dongle = new_dongle;
		id++;
	}
}

t_team	*create_coders(int nb)
{
	t_team		*team;

	team = alloc_coders(nb);
	team->nb = nb;
	assign_mutex(team);
	assign_threads(team);
	assign_dongles(team);
	return (team);
}
