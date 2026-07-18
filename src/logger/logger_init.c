/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 00:34:09 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 13:03:07 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"
#include <stdlib.h>

t_log	*logger_pop(t_logger *logger)
{
	t_log	**new_list;
	t_log	*first;
	int		i;

	if (logger->size < 1)
		return (NULL);
	first = logger->waitlist[0];
	new_list = NULL;
	if (logger->size > 1)
	{
		new_list = ft_calloc(logger->size - 1, sizeof(t_log *));
		if (!new_list)
			return (NULL);
		i = 0;
		while (i < logger->size - 1)
		{
			new_list[i] = logger->waitlist[i + 1];
			i++;
		}
	}
	free(logger->waitlist);
	logger->waitlist = new_list;
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
	while (i < logger->size)
	{
		if (logger->waitlist && logger->waitlist[i])
		{
			free(logger->waitlist[i]->text);
			free(logger->waitlist[i]);
		}
		i++;
	}
	free(logger->waitlist);
	pthread_mutex_destroy(&logger->lock);
	pthread_cond_destroy(&logger->has_log);
	free(logger);
}
