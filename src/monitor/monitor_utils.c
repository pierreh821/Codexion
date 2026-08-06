/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 17:34:50 by phenry            #+#    #+#             */
/*   Updated: 2026/08/06 06:19:01 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	check_burnout(t_table *table, int id)
{
	t_coder		*coder;
	t_task		task;
	long		start;

	coder = table->team->coders_list[id];
	pthread_mutex_lock(&coder->task_lock);
	task = coder->task;
	start = coder->last_compile_start;
	pthread_mutex_unlock(&coder->task_lock);
	if (task != SUSPEND
		&& get_time_ms() - start > table->args->time_to_burnout)
	{
		logger_write(coder, "burned out");
		request_stop(table, STOP_BURNOUT, id);
	}
}

void	*routine(void *arg)
{
	t_table	*table;
	int		i;

	table = (t_table *)arg;
	while (is_running(table))
	{
		i = 0;
		while (i < table->team->nb)
			check_burnout(table, i++);
		sliced_sleep(table, 1000);
	}
	return (NULL);
}

long	time_elapsed(t_monitor *monitor)
{
	long	res;

	pthread_mutex_lock(&monitor->lock);
	res = get_time_ms() - monitor->sim_start;
	pthread_mutex_unlock(&monitor->lock);
	return (res);
}

int	is_running(t_table *table)
{
	int	running;

	pthread_mutex_lock(&table->status->lock);
	running = (table->status->reason == STOP_NONE);
	pthread_mutex_unlock(&table->status->lock);
	return (running);
}
