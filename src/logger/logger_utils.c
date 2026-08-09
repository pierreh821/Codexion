/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 17:22:22 by phenry            #+#    #+#             */
/*   Updated: 2026/08/06 03:31:58 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int	extend_waitlist_logger(t_logger *logger, t_log *log)
{
	t_log	**new_list;
	int		i;

	new_list = ft_calloc(logger->size + 1, sizeof(t_log *));
	if (!new_list)
		return (0);
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
	return (1);
}

void	logger_write(t_coder *coder, char *text)
{
	t_log		*log;
	t_logger	*logger;

	log = create_log(coder, text);
	if (!log)
	{
		request_stop(coder->table, STOP_FATAL, coder->id);
		return ;
	}
	logger = coder->table->monitor->logger;
	log->logger = logger;
	pthread_mutex_lock(&logger->lock);
	if (!extend_waitlist_logger(logger, log))
	{
		pthread_mutex_unlock(&logger->lock);
		free(log->text);
		free(log);
		request_stop(coder->table, STOP_FATAL, coder->id);
		return ;
	}
	pthread_cond_signal(&logger->has_log);
	pthread_mutex_unlock(&logger->lock);
}

int	logger_check_run(t_table *table, t_logger *logger)
{
	int	running;

	running = is_running(table);
	pthread_mutex_lock(&logger->lock);
	while (logger->size == 0 && running)
	{
		pthread_mutex_unlock(&logger->lock);
		running = is_running(table);
		pthread_mutex_lock(&logger->lock);
		if (logger->size == 0 && running)
			pthread_cond_wait(&logger->has_log, &logger->lock);
	}
	if (logger->size == 0 && !running)
	{
		pthread_mutex_unlock(&logger->lock);
		return (0);
	}
	return (1);
}

int	log_export_cycle(t_log *log, t_logger *logger, int *stop)
{
	log = logger_pop(logger);
	if (log)
	{
		printf("%d %d %s\n", log->timestamp, log->id, log->text);
		if (strcmp(log->text, "burned out") == 0)
			*stop = 1;
		free(log->text);
		free(log);
		if (*stop)
			return (0);
	}
	return (1);
}

void	*log_export(void *arg)
{
	t_table		*table;
	t_logger	*logger;
	t_log		*log;
	int			stop;

	table = (t_table *)arg;
	logger = table->monitor->logger;
	log = NULL;
	stop = 0;
	while (!stop)
	{
		if (!logger_check_run(table, logger))
			break ;
		while (logger->size > 0)
		{
			if (!log_export_cycle(log, logger, &stop))
				break ;
		}
		pthread_mutex_unlock(&logger->lock);
	}
	return (NULL);
}
