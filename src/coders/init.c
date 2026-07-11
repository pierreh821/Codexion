/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 03:23:32 by phenry            #+#    #+#             */
/*   Updated: 2026/07/11 16:23:57 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_team	*alloc_coders(int nb)
{
	t_team	*team;

	team = malloc(sizeof(t_team));
	if (!team)
		error("Cannot allocate memory for team");
	team->nb = nb;
	team->coders_list = malloc(sizeof(t_coder *) * nb);
	if (!team->coders_list)
		error("Cannot allocate memory for coders list");
	team->dongle_set = malloc(sizeof(t_dongle *) * nb);
	if (!team->dongle_set)
		error("Cannot allocate memeory for dongle set");
	return (team);
}

void	assign_cond(t_team *team)
{
	if (pthread_cond_init(&(team->run), NULL) != 0)
		error("Failed to init run condition (team)");
	if (pthread_mutex_init(&(team->run_lock), NULL) != 0)
		error("Failed to init run_lock mutex (team)");
	team->run_signal = 0;
}

void	assign_coders(t_table *table, void *(*work)(void *))
{
	int		i;
	t_team	*team;

	i = 0;
	team = table->team;
	while (i < table->team->nb)
	{
		team->coders_list[i] = malloc(sizeof(t_coder));
		team->coders_list[i]->id = i + 1;
		team->coders_list[i]->run = &(team->run);
		team->coders_list[i]->run_lock = &(team->run_lock);
		team->coders_list[i]->run_signal = &(team->run_signal);
		team->coders_list[i]->time = &(table->monitor->time);
		team->coders_list[i]->table = table;
		if (pthread_create(&team->coders_list[i]->thread_id, NULL, work,
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
		team->dongle_set[id] = new_dongle;
		team->coders_list[id]->left_dongle = new_dongle;
		if (id > 0)
			team->coders_list[id - 1]->right_dongle = new_dongle;
		else
			team->coders_list[team->nb - 1]->right_dongle = new_dongle;
		id++;
	}
}

t_team	*create_team(t_table *table, void *(*work)(void *))
{
	t_team		*team;

	team = alloc_coders(table->args->number_of_coders);
	assign_cond(team);
	table->team = team;
	assign_coders(table, work);
	assign_dongles(team);
	return (team);
}
