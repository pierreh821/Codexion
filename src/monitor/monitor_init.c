/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 19:40:33 by phenry            #+#    #+#             */
/*   Updated: 2026/07/13 19:12:27 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	end_wait_monitor(t_monitor *monitor)
{
	monitor->run = 0;
	pthread_join(monitor->thread_id, NULL);
	pthread_join(monitor->logger_id, NULL);
	free_logger(monitor->logger);
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
	table->monitor->logger = init_logger();
	if (pthread_create(&table->monitor->logger_id, NULL,
			log_export, table) != 0)
		error("Cannot create logger thread");
}
