/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 00:34:09 by phenry            #+#    #+#             */
/*   Updated: 2026/07/13 01:28:50 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	ft_strlcpy(char *dest, const char *src)
{
	size_t	i;

	i = 0;
	dest = malloc(sizeof(char) * strlen(src));
	if (!dest)
		error("Failed to malloc for strlcpy");
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = 0;
}

void	add_to_list(t_logger *log, char *text)
{
	char	**swap;
	int		i;

	i = 0;
	swap = malloc(sizeof(char *) * log->size);
	if (!swap)
		error("Failed to allocate memory for swap (log/add_to_list)");
	while (i < log->size)
	{
		ft_strlcpy(swap[i], log->waitlist[i]);
		i++;
	}
	free(log->waitlist);
	log->waitlist = malloc(sizeof(char *) * (log->size + 1));
	if (!log->waitlist)
		error("Failed to allocate memory for log->waitlist (log/add_to_list)");
	i = 0;
	while (i < log->size)
	{
		ft_strlcpy(log->waitlist[i], swap[i]);
		i++;
	}
	ft_strlcpy(log->waitlist[i], text);
}

t_logger	*init_logger(void)
{
	t_logger	*logger;

	logger = malloc(sizeof(t_logger));
	logger->waitlist = malloc(0);
	if (!logger)
		error("Failed to allocate memory for logger waitlist");
	if (pthread_mutex_init(&logger->lock, NULL) != 0)
		error("Failed to create logger mutex");
	logger->size = 0;
	return (logger);
}

void	free_logger(t_logger *logger)
{
	free(logger->waitlist);
	pthread_mutex_destroy(&logger->lock);
	free(logger);
}

void	new_log(t_logger *logger, char	*text)
{
	pthread_mutex_lock(&logger->lock);
	add_to_list(logger, text);
	pthread_mutex_unlock(&logger->lock);
}

