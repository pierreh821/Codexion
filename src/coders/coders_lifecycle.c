/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_lifecycle.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 02:09:41 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 03:26:20 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	wait_for_start(t_coder *coder)
{
	pthread_mutex_lock(coder->run_lock);
	while (*(coder->run_signal) == 0)
	{
		pthread_cond_wait(coder->run, coder->run_lock);
	}
	pthread_mutex_unlock(coder->run_lock);
}

void	wait_team(t_team *team)
{
	int	id;

	id = 0;
	while (id < team->nb)
	{
		pthread_join(team->coders_list[id]->thread_id, NULL);
		id++;
	}
}

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
