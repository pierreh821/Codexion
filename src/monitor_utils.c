/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 19:40:33 by phenry            #+#    #+#             */
/*   Updated: 2026/07/13 01:59:59 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	check_burnout(t_table *table, int id)
{
	long	now;
	long	start;

	now = get_time_ms();
	start = table->team->coders_list[id]->start;
	if (now - start > table->args->time_to_burnout)
	{
		printf("Coder %d burnout\n", table->team->coders_list[id]->id + 1);
		printf("now: %ld, start: %ld\n", now, start);
		error("burnout\n");
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

void	end_wait_monitor(t_monitor *monitor)
{
	monitor->run = 0;
	pthread_join(monitor->thread_id, NULL);
	free_logger(monitor->logger);
	free(monitor);
}

long	time_elapsed(t_monitor *monitor)
{
	return (get_time_ms() - monitor->start);
}

void	create_monitor(t_table *table)
{
	table->monitor = malloc(sizeof(t_monitor));
	if (!table->monitor)
		error("Cannot allocate memory for monitor");
	table->monitor->run = 1;
	if (pthread_create(&table->monitor->thread_id, NULL, routine, table) != 0)
		error("Cannot create monitor thread");
	table->monitor->logger = init_logger();
	table->monitor->elapsed = time_elapsed;
}
