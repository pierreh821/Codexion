/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_lifecycle.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 02:09:41 by phenry            #+#    #+#             */
/*   Updated: 2026/08/06 04:54:51 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	wait_for_start(t_coder *coder)
{
	pthread_mutex_lock(coder->run_lock);
	while (*(coder->run_signal) == 0 && is_running(coder->table))
		pthread_cond_wait(coder->run, coder->run_lock);
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

	pthread_mutex_lock(&(table->team->run_lock));
	table->team->run_signal = 1;
	pthread_cond_broadcast(&(table->team->run));
	pthread_mutex_unlock(&(table->team->run_lock));
	i = 0;
	while (i < table->team->nb)
	{
		set_task(table->team->coders_list[i], ACQUIRING, 1);
		i++;
	}
	table->monitor->sim_start = get_time_ms();
}

void	set_task(t_coder *coder, t_task task, int update_start)
{
	pthread_mutex_lock(&coder->task_lock);
	coder->task = task;
	if (update_start)
		coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->task_lock);
}
