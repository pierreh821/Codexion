/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 19:40:33 by phenry            #+#    #+#             */
/*   Updated: 2026/07/13 17:03:35 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	check_burnout(t_table *table, int id)
{
	t_state	state;
	long	now;
	long	start;

	now = get_time_ms();
	start = table->team->coders_list[id]->start;
	state = table->team->coders_list[id]->state;
	if (state != SUSPEND && now - start > table->args->time_to_burnout)
	{
		printf("Coder %d burnout\n", table->team->coders_list[id]->id + 1);
		printf("now: %ld, start: %ld\n", now, start);
		error("burnout\n");
	}
}

void	*print_log(void *arg)
{
	t_table		*table;
	t_logger	*logger;
	t_log		*log;

	table = (t_table *)arg;
	logger = table->monitor->logger;
	while (table->monitor->run || logger->size > 0)
	{
		pthread_mutex_lock(&logger->lock);
		while (logger->size > 0)
		{
			log = logger_pop(logger);
			if (log)
			{
				printf("%d %d %s\n", log->timestamp, log->id, log->text);
				free(log->text);
				free(log);
			}
		}
		pthread_mutex_unlock(&logger->lock);
		usleep(500);
	}
	return (NULL);
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
	pthread_join(monitor->logger_id, NULL);
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
	if (pthread_create(&table->monitor->logger_id, NULL, print_log, table) != 0)
		error("Cannot create logger thread");
}
