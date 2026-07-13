/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 17:51:59 by phenry            #+#    #+#             */
/*   Updated: 2026/07/13 02:21:34 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MONITOR_H
# define MONITOR_H
# include <pthread.h>
# include <sys/time.h>
# include "logger.h"

typedef struct s_monitor
{
	pthread_t		thread_id;
	long			start;
	int				run;
	long int		(*elapsed)(struct s_monitor *monitor);
	t_logger		*logger;
	pthread_t		logger_id;
}	t_monitor;

#endif
