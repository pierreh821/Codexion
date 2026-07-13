/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 00:30:39 by phenry            #+#    #+#             */
/*   Updated: 2026/07/13 19:40:12 by phenry           ###   ########.fr       */
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

void	set_status(t_coder *coder, t_status status, int update_start)
{
	pthread_mutex_lock(&coder->status_lock);
	coder->status = status;
	if (update_start)
		coder->start = get_time_ms();
	pthread_mutex_unlock(&coder->status_lock);
}

void	free_team(t_team *team)
{
	int	id;

	id = 0;
	while (id < team->nb)
	{
		pthread_join(team->coders_list[id]->thread_id, NULL);
		pthread_mutex_destroy(&team->coders_list[id]->status_lock);
		free(team->coders_list[id]);
		id++;
	}
	free_dongles(team->dongle_set, team->nb);
	free(team->coders_list);
	pthread_mutex_destroy(&(team->run_lock));
	pthread_cond_destroy(&(team->run));
	free(team);
}
