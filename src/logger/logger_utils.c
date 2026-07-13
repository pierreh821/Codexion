/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 17:22:22 by phenry            #+#    #+#             */
/*   Updated: 2026/07/13 17:41:02 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

char	*ft_strdup(const char *src)
{
	char	*dest;
	int		i;

	dest = malloc((sizeof(char) * strlen(src) + 1));
	if (dest == NULL)
		error("Malloc strdup error");
	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

void	add_to_list(t_logger *logger, t_log *log)
{
	t_log	**new_list;
	int		i;

	new_list = malloc(sizeof(t_log *) * (logger->size + 1));
	if (!new_list)
		error("Failed to allocate memory for add_to_list log");
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

	log = malloc(sizeof(t_log));
	if (!log)
		error("Failed to malloc log");
	log->id = coder->id;
	log->timestamp = time_elapsed(coder->table->monitor);
	log->text = ft_strdup(text);
	log->logger = coder->table->monitor->logger;
	pthread_mutex_lock(&log->logger->lock);
	add_to_list(log->logger, log);
	pthread_mutex_unlock(&log->logger->lock);
}
