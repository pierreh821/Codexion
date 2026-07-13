/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 00:35:05 by phenry            #+#    #+#             */
/*   Updated: 2026/07/13 17:20:40 by phenry           ###   ########.fr       */
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
	t_log			**waitlist;
	int				size;
}	t_logger;

#endif
