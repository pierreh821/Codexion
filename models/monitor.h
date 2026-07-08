/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 17:51:59 by phenry            #+#    #+#             */
/*   Updated: 2026/07/08 18:23:35 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MONITOR_H
# define MONITOR_H
# include <pthread.h>
# include <time.h>

typedef struct s_monitor
{
	pthread_t		thread_id;
	struct timeval	start_tm;
	struct timeval	tm;
}	t_monitor;

#endif
