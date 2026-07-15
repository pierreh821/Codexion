/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 03:23:32 by phenry            #+#    #+#             */
/*   Updated: 2026/07/15 11:24:54 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

t_team	*alloc_coders(int nb)
{
	t_team	*team;

	team = ft_calloc(1, sizeof(t_team));
	if (!team)
		error("Cannot allocate memory for team");
	team->nb = nb;
	team->coders_list = ft_calloc(nb, sizeof(t_coder *));
	if (!team->coders_list)
		error("Cannot allocate memory for coders list");
	team->dongle_set = ft_calloc(nb, sizeof(t_dongle *));
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

void	assign_coders(t_table *table)
{
	int		i;
	t_team	*team;

	i = 0;
	team = table->team;
	while (i < table->team->nb)
	{
		team->coders_list[i] = ft_calloc(1, sizeof(t_coder));
		team->coders_list[i]->id = i + 1;
		team->coders_list[i]->run = &(team->run);
		team->coders_list[i]->run_lock = &(team->run_lock);
		team->coders_list[i]->run_signal = &(team->run_signal);
		team->coders_list[i]->table = table;
		if (pthread_mutex_init(&team->coders_list[i]->task_lock, NULL) != 0)
			error("Failed to init task_lock mutex");
		set_task(team->coders_list[i], SUSPEND, 1);
		i++;
	}
}

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

t_team	*create_team(t_table *table, void *(*work)(void *))
{
	t_team		*team;

	team = alloc_coders(table->args->number_of_coders);
	assign_cond(team);
	table->team = team;
	assign_coders(table);
	assign_dongles(team);
	launch_threads(table, work);
	return (team);
}
