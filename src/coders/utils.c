/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 00:30:39 by phenry            #+#    #+#             */
/*   Updated: 2026/07/12 18:18:42 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	team_start(t_table *table)
{
	int	i;

	pthread_mutex_lock(&(table->team->run_lock));
	table->team->run_signal = 1;
	pthread_cond_broadcast(&(table->team->run));
	pthread_mutex_unlock(&(table->team->run_lock));
	i = 0;
	while (i < table->team->nb)
	{
		gettimeofday(&table->team->coders_list[i]->start, NULL);
		i++;
	}
	gettimeofday(&table->monitor->start, NULL);
}

void	team_pause(t_team *team)
{
	pthread_mutex_lock(&(team->run_lock));
	team->run_signal = 0;
	pthread_cond_broadcast(&(team->run));
	pthread_mutex_unlock(&(team->run_lock));
}

void	free_team(t_team *team)
{
	int	id;

	id = 0;
	while (id < team->nb)
	{
		pthread_join(team->coders_list[id]->thread_id, NULL);
		free_dongle(team->coders_list[id]->left_dongle);
		free(team->coders_list[id]);
		id++;
	}
	free(team->coders_list);
	free(team->dongle_set);
	pthread_mutex_destroy(&(team->run_lock));
	pthread_cond_destroy(&(team->run));
	free(team);
}
