/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 17:34:50 by phenry            #+#    #+#             */
/*   Updated: 2026/07/13 19:27:02 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	check_burnout(t_table *table, int id)
{
	t_status	status;
	long		now;
	long		start;

	now = get_time_ms();
	start = table->team->coders_list[id]->start;
	status = table->team->coders_list[id]->status;
	if (status != SUSPEND && now - start > table->args->time_to_burnout)
	{
		logger_write(table->team->coders_list[id], "burned out");
		table->team->run_signal = 0;
		table->monitor->run = 0;
		error("");
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
