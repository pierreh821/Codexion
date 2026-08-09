/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 00:34:09 by phenry            #+#    #+#             */
/*   Updated: 2026/08/06 03:54:25 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"
#include <stdlib.h>

t_log	*logger_pop(t_logger *logger)
{
	t_log	*first;
	int		i;

	if (logger->size < 1)
		return (NULL);
	first = logger->waitlist[0];
	i = 0;
	while (i < logger->size - 1)
	{
		logger->waitlist[i] = logger->waitlist[i + 1];
		i++;
	}
	logger->waitlist[i] = NULL;
	logger->size--;
	return (first);
}

t_logger	*init_logger(void)
{
	t_logger	*logger;

	logger = ft_calloc(1, sizeof(t_logger));
	if (!logger)
		return (NULL);
	logger->waitlist = NULL;
	if (!logger)
		return (NULL);
	if (pthread_mutex_init(&logger->lock, NULL) != 0)
		return (free(logger), NULL);
	if (pthread_cond_init(&logger->has_log, NULL) != 0)
	{
		pthread_mutex_destroy(&logger->lock);
		free(logger);
		return (NULL);
	}
	logger->size = 0;
	return (logger);
}

void	free_logger(t_logger *logger)
{
	int	i;

	i = 0;
	if (!logger)
		return ;
	free_waitlist(logger->waitlist, logger->size);
	pthread_mutex_destroy(&logger->lock);
	pthread_cond_destroy(&logger->has_log);
	free(logger);
}

void	free_waitlist(t_log **waitlist, int size)
{
	int	i;

	i = 0;
	if (!waitlist)
		return ;
	while (i < size)
	{
		if (waitlist[i])
		{
			free(waitlist[i]->text);
			free(waitlist[i]);
		}
		i++;
	}
	free(waitlist);
}

t_log	*create_log(t_coder *coder, char *text)
{
	t_log	*log;

	log = ft_calloc(1, sizeof(t_log));
	if (!log)
		return (NULL);
	log->id = coder->id;
	log->timestamp = time_elapsed(coder->table->monitor);
	log->text = ft_strdup(text);
	if (!log->text)
	{
		free(log);
		return (NULL);
	}
	return (log);
}
