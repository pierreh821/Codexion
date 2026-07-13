/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 00:34:09 by phenry            #+#    #+#             */
/*   Updated: 2026/07/13 02:38:28 by phenry           ###   ########.fr       */
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

void	free_char_pp(char **list, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		free(list[i]);
		i++;
	}
	free(list);
}

void	add_to_list(t_logger *logger, char *text)
{
	char	**new_list;
	int		i;

	new_list = malloc(sizeof(char *) * (logger->size + 1));
	if (!new_list)
		error("Failed to allocate memory for add_to_list log");
	i = 0;
	while (i < logger->size)
	{
		new_list[i] = logger->waitlist[i];
		i++;
	}
	new_list[i] = ft_strdup(text);
	free(logger->waitlist);
	logger->waitlist = new_list;
	logger->size++;
}

char	*logger_pop(t_logger *logger)
{
	char	**new_list;
	char	*first;
	int		i;

	if (logger->size == 0)
		return (NULL);
	first = logger->waitlist[0];
	new_list = NULL;
	if (logger->size > 1)
	{
		new_list = malloc(sizeof(char *) * (logger->size - 1));
		if (!new_list)
			error("Failed to allocate memory in logger pop");
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

static int	fill_nbr(char *buf, long nbr, int idx)
{
	if (nbr == 0)
		buf[--idx] = '0';
	while (nbr > 0)
	{
		buf[--idx] = (nbr % 10) + '0';
		nbr /= 10;
	}
	return (idx);
}

void	logger_write(t_coder *coder, const char *msg)
{
	char	buf[256];
	int		idx;
	int		i;
	long	time;

	memset(buf, 0, 256);
	time = coder->table->monitor->elapsed(coder->table->monitor);
	idx = 254;
	buf[idx] = '\n';
	i = strlen(msg);
	while (--i >= 0)
		buf[--idx] = msg[i];
	buf[--idx] = ' ';
	idx = fill_nbr(buf, coder->id, idx);
	buf[--idx] = ' ';
	idx = fill_nbr(buf, time, idx);
	new_log(coder->table->monitor->logger, &buf[idx]);
}
