/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 00:35:05 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 12:20:28 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_H
# define LOGGER_H

# include "pthread.h"

typedef struct s_logger	t_logger;

typedef struct s_log
{
	int			timestamp;
	int			id;
	char		*text;
	t_logger	*logger;
}	t_log;

typedef struct s_logger
{
	pthread_mutex_t	lock;
	pthread_cond_t	has_log;
	t_log			**waitlist;
	int				size;
}	t_logger;

t_log		*logger_pop(t_logger *logger);
t_logger	*init_logger(void);
void		free_logger(t_logger *logger);

void		extend_waitlist_logger(t_table *table, t_logger *logger,
				t_log *log);
void		logger_write(t_coder *coder, char *text);
void		*log_export(void *arg);

#endif
