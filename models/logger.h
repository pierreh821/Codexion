/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 00:35:05 by phenry            #+#    #+#             */
/*   Updated: 2026/07/13 00:40:44 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_H
# define LOGGER_H

# include "pthread.h"

typedef struct s_logger
{
	pthread_mutex_t	lock;
	char			**waitlist;
	int				size;
}	t_logger;

#endif
