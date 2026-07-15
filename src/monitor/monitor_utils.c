/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 17:34:50 by phenry            #+#    #+#             */
/*   Updated: 2026/07/15 23:13:31 by phenry           ###   ########.fr       */
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
	start = coder->start;
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
	while (table->monitor->run)
	{
		i = 0;
		while (i < table->team->nb)
			check_burnout(table, i++);
		usleep(1000);
	}
	return (NULL);
}

long	time_elapsed(t_monitor *monitor)
{
	return (get_time_ms() - monitor->start);
}
