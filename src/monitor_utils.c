/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 19:40:33 by phenry            #+#    #+#             */
/*   Updated: 2026/07/11 20:39:49 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	check_burnout(t_table *table, int id)
{
	int		now;
	int		start;
	t_state	state;

	now = table->monitor->time.tv_usec;
	start = table->team->coders_list[id]->start.tv_usec;
	state = table->team->coders_list[id]->state;
	if (state == COMPILING && now - start > table->args->time_to_burnout)
		error("Coder burnout");
}

void	*routine(void *arg)
{
	t_table	*table;
	int		i;

	table = (t_table *)arg;
	while (table->monitor->run)
	{
		gettimeofday(&table->monitor->time, NULL);
		i = 0;
		while (i < table->team->nb)
			check_burnout(table, i++);
		usleep(1000);
	}
	return (NULL);
}

void	end_wait_monitor(t_monitor *monitor)
{
	monitor->run = 0;
	pthread_join(monitor->thread_id, NULL);
	free(monitor);
}

void	create_monitor(t_table *table)
{
	table->monitor = malloc(sizeof(t_monitor));
	if (!table->monitor)
		error("Cannot allocate memory for monitor");
	table->monitor->run = 1;
	if (pthread_create(&table->monitor->thread_id, NULL, routine, table) != 0)
		error("Cannot create monitor thread");
}
