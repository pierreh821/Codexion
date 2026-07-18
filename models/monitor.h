/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 17:51:59 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 03:08:13 by phenry           ###   ########.fr       */
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
	t_logger		*logger;
	pthread_t		logger_id;
}	t_monitor;

void	free_monitor(t_monitor *monitor);
void	wait_monitor(t_monitor *monitor);
void	create_monitor(t_table *table);

void	check_burnout(t_table *table, int id);
void	*routine(void *arg);
long	time_elapsed(t_monitor *monitor);
int		is_running(t_table *table);

#endif
