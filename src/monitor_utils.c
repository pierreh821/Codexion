/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 19:40:33 by phenry            #+#    #+#             */
/*   Updated: 2026/07/13 01:45:30 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	check_burnout(t_table *table, int id)
{
	int		now;
	int		start;

		now = table->monitor->time.tv_usec * 1000;
	start = table->team->coders_list[id]->start.tv_usec * 1000;
		if (now - start > table->args->time_to_burnout)
		{
		printf("Coder %d burnout\n", table->team->coders_list[id]->id + 1);
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
	free_logger(monitor->logger);
	free(monitor);
}

long int	time_elapsed(t_monitor *monitor)
{
	long int	start;
	long int	now;

	gettimeofday(&monitor->time, NULL);
	start = monitor->start.tv_sec * 1000L + monitor->start.tv_usec / 1000;
	now = monitor->time.tv_sec * 1000L + monitor->time.tv_usec / 1000;
	return (now - start);
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
