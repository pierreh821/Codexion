/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 03:23:32 by phenry            #+#    #+#             */
/*   Updated: 2026/07/15 22:40:22 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

int	alloc_team(t_table *table)
{
	int		nb;

	nb = table->args->number_of_coders;
	table->team = ft_calloc(1, sizeof(t_team));
	if (!table->team)
		return (free_table(table), 0);
	table->team->nb = nb;
	table->team->coders_list = ft_calloc(nb, sizeof(t_coder *));
	if (!table->team->coders_list)
		return (free_table(table), 0);
	table->team->dongle_set = ft_calloc(nb, sizeof(t_dongle *));
	if (!table->team->dongle_set)
		return (free_table(table), 0);
	return (0);
}

int	assign_cond(t_table *table)
{
	if (pthread_cond_init(&(table->team->run), NULL) != 0)
		return (1);
	if (pthread_mutex_init(&(table->team->run_lock), NULL) != 0)
		return (1);
	table->team->run_signal = 0;
	return (0);
}

int	assign_coders(t_table *table)
{
	int		i;
	t_team	*team;

	i = 0;
	team = table->team;
	while (i < table->team->nb)
	{
		team->coders_list[i] = ft_calloc(1, sizeof(t_coder));
		if (!team->coders_list[i])
			return (1);
		team->coders_list[i]->id = i + 1;
		team->coders_list[i]->run = &(team->run);
		team->coders_list[i]->run_lock = &(team->run_lock);
		team->coders_list[i]->run_signal = &(team->run_signal);
		team->coders_list[i]->table = table;
		if (pthread_mutex_init(&team->coders_list[i]->task_lock, NULL) != 0)
			return (1);
		set_task(team->coders_list[i], SUSPEND, 1);
		i++;
	}
	return (0);
}

int	launch_threads(t_table *table, void *(*work)(void *))
{
	int	i;

	i = 0;
	while (i < table->team->nb)
	{
		if (pthread_create(&table->team->coders_list[i]->thread_id, NULL, work,
				table->team->coders_list[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}

t_team	*create_team(t_table *table, void *(*work)(void *))
{
	int			res;

	res = 0;
	res += alloc_team(table);
	res += assign_cond(table);
	res += assign_coders(table);
	res += assign_dongles(table->team);
	res += launch_threads(table, work);
	if (res > 0)
		return (free_table(table), NULL);
	return (table->team);
}
