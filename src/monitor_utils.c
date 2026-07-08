/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 19:40:33 by phenry            #+#    #+#             */
/*   Updated: 2026/07/08 20:06:05 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "monitor.h"
#include "codexion.h"

void	*eval_time(void *arg)
{
	t_monitor	*monitor;

	monitor = (t_monitor *)arg;
	while (1)
	{
		gettimeofday(&monitor->tm, NULL);
		usleep(1000);
	}
}

t_monitor	*create_monitor(void)
{
	t_monitor	*monitor;

	monitor = malloc(sizeof(t_monitor));
	if (pthread_create(&monitor->thread_id, NULL, eval_time, monitor) != 0)
		error("Cannot create monitor thread");
	return (monitor);
}

