/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 19:40:33 by phenry            #+#    #+#             */
/*   Updated: 2026/08/06 06:18:06 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"
#include <stdlib.h>

void	free_monitor(t_monitor *monitor)
{
	if (!monitor)
		return ;
	if (monitor->logger)
		free_logger(monitor->logger);
	pthread_mutex_destroy(&monitor->lock);
	free(monitor);
}

void	wait_monitor(t_monitor *monitor)
{
	pthread_join(monitor->thread_id, NULL);
	pthread_join(monitor->logger_id, NULL);
}

int	monitor_error(t_table *table)
{
	free_monitor(table->monitor);
	table->monitor = NULL;
	return (0);
}

int	create_monitor(t_table *table)
{
	table->monitor = ft_calloc(1, sizeof(t_monitor));
	if (!table->monitor)
		return (0);
	table->monitor->logger = init_logger();
	if (!table->monitor->logger)
		return (monitor_error(table));
	if (pthread_create(&table->monitor->thread_id, NULL, routine, table) != 0)
		return (monitor_error(table));
	if (pthread_create(&table->monitor->logger_id, NULL,
			log_export, table) != 0)
	{
		pthread_join(table->monitor->thread_id, NULL);
		free_monitor(table->monitor);
		table->monitor = NULL;
		return (0);
	}
	if (pthread_mutex_init(&table->monitor->lock, NULL) != 0)
	{
		pthread_join(table->monitor->thread_id, NULL);
		free_monitor(table->monitor);
		table->monitor = NULL;
		return (0);
	}
	return (1);
}
