/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 19:40:33 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 03:05:12 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"
#include <stdlib.h>

void	free_monitor(t_monitor *monitor)
{
	if (!monitor)
		return ;
	free_logger(monitor->logger);
	free(monitor);
}

void	wait_monitor(t_monitor *monitor)
{
	pthread_join(monitor->thread_id, NULL);
	pthread_join(monitor->logger_id, NULL);
}

void	create_monitor(t_table *table)
{
	table->monitor = ft_calloc(1, sizeof(t_monitor));
	if (!table->monitor)
		return (free_table(table));
	table->monitor->run = 1;
	if (pthread_create(&table->monitor->thread_id, NULL, routine, table) != 0)
		return (free_table(table));
	table->monitor->logger = init_logger();
	if (pthread_create(&table->monitor->logger_id, NULL,
			log_export, table) != 0)
		return (free_table(table));
}
