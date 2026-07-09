/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 19:40:33 by phenry            #+#    #+#             */
/*   Updated: 2026/07/09 17:49:29 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*eval_time(void *arg)
{
	t_monitor	*monitor;

	monitor = (t_monitor *)arg;
	while (monitor->run)
	{
		gettimeofday(&monitor->tm, NULL);
		usleep(1000);
	}
	return (NULL);
}

t_monitor	*create_monitor(void)
{
	t_monitor	*monitor;

	monitor = malloc(sizeof(t_monitor));
	if (pthread_create(&monitor->thread_id, NULL, eval_time, monitor) != 0)
		error("Cannot create monitor thread");
	monitor->run = 1;
	return (monitor);
}

void	end_wait_monitor(t_monitor *monitor)
{
	monitor->run = 0;
	pthread_join(monitor->thread_id, NULL);
	free(monitor);
}
