/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 00:30:39 by phenry            #+#    #+#             */
/*   Updated: 2026/07/15 12:37:09 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	team_start(t_table *table)
{
	int		i;
	long	start_time;

	pthread_mutex_lock(&(table->team->run_lock));
	table->team->run_signal = 1;
	pthread_cond_broadcast(&(table->team->run));
	pthread_mutex_unlock(&(table->team->run_lock));
	start_time = get_time_ms();
	i = 0;
	while (i < table->team->nb)
	{
		table->team->coders_list[i]->start = start_time;
		i++;
	}
	table->monitor->start = get_time_ms();
}

void	team_pause(t_team *team)
{
	pthread_mutex_lock(&(team->run_lock));
	team->run_signal = 0;
	pthread_cond_broadcast(&(team->run));
	pthread_mutex_unlock(&(team->run_lock));
}

void	set_task(t_coder *coder, t_task task, int update_start)
{
	pthread_mutex_lock(&coder->task_lock);
	coder->task = task;
	if (update_start)
		coder->start = get_time_ms();
	pthread_mutex_unlock(&coder->task_lock);
}

void	free_team(t_team *team)
{
	int	id;

	if (!team)
		return ;
	id = 0;
	if (team->coders_list)
	{
		while (id < team->nb)
		{
			if (team->coders_list[id])
			{
				pthread_mutex_destroy(&team->coders_list[id]->task_lock);
				free(team->coders_list[id]);
			}
			id++;
		}
		free(team->coders_list);
	}
	free_dongles(team->dongle_set, team->nb);
	pthread_mutex_destroy(&(team->run_lock));
	pthread_cond_destroy(&(team->run));
	free(team);
}
