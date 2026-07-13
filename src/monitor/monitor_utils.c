/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 17:34:50 by phenry            #+#    #+#             */
/*   Updated: 2026/07/13 17:51:31 by phenry           ###   ########.fr       */
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

long	time_elapsed(t_monitor *monitor)
{
	return (get_time_ms() - monitor->start);
}
