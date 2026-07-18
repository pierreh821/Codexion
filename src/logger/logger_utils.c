/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 17:22:22 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 15:24:39 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"
#include <stdlib.h>
#include <stdio.h>

void	extend_waitlist_logger(t_table *table, t_logger *logger, t_log *log)
{
	t_log	**new_list;
	int		i;

	new_list = ft_calloc(logger->size + 1, sizeof(t_log *));
	if (!new_list)
		request_stop(table, STOP_FATAL, log->id);
	i = 0;
	while (i < logger->size)
	{
		new_list[i] = logger->waitlist[i];
		i++;
	}
	new_list[i] = log;
	free(logger->waitlist);
	logger->waitlist = new_list;
	logger->size++;
}

void	logger_write(t_coder *coder, char *text)
{
	t_log		*log;

	log = ft_calloc(1, sizeof(t_log));
	if (!log)
	{
		request_stop(coder->table, STOP_FATAL, coder->id);
		return ;
	}
	log->id = coder->id;
	log->timestamp = time_elapsed(coder->table->monitor);
	log->text = ft_strdup(text);
	if (!log->text)
	{
		free(log);
		request_stop(coder->table, STOP_FATAL, coder->id);
		return ;
	}
	log->logger = coder->table->monitor->logger;
	pthread_mutex_lock(&log->logger->lock);
	extend_waitlist_logger(coder->table, log->logger, log);
	pthread_cond_signal(&log->logger->has_log);
	pthread_mutex_unlock(&log->logger->lock);
}

void	*log_export(void *arg)
{
	t_table		*table;
	t_logger	*logger;
	t_log		*log;

	table = (t_table *)arg;
	logger = table->monitor->logger;
	pthread_mutex_lock(&logger->lock);
	while (is_running(table) || logger->size > 0)
	{
		while (logger->size == 0 && is_running(table))
			pthread_cond_wait(&logger->has_log, &logger->lock);
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
	}
	pthread_mutex_unlock(&logger->lock);
	return (NULL);
}
