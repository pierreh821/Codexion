/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 17:22:22 by phenry            #+#    #+#             */
/*   Updated: 2026/07/15 22:37:28 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

char	*ft_strdup(const char *src)
{
	char	*dest;
	int		i;

	dest = ft_calloc(strlen(src) + 1, sizeof(char));
	if (dest == NULL)
		return (NULL);
	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

void	add_to_list(t_table *table, t_logger *logger, t_log *log)
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
		request_stop(coder->table, STOP_FATAL, coder->id);
	log->id = coder->id;
	log->timestamp = time_elapsed(coder->table->monitor);
	log->text = ft_strdup(text);
	if (!log->text)
		request_stop(coder->table, STOP_FATAL, coder->id);
	log->logger = coder->table->monitor->logger;
	pthread_mutex_lock(&log->logger->lock);
	add_to_list(coder->table, log->logger, log);
	pthread_mutex_unlock(&log->logger->lock);
}

void	*log_export(void *arg)
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
