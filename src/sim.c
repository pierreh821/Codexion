/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 19:46:13 by phenry            #+#    #+#             */
/*   Updated: 2026/07/08 19:48:20 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <sys/time.h>
#include "monitor.h"
#include "coders.h"

void	start_simulation(t_monitor *monitor, t_team *team)
{
	pthread_mutex_unlock(&(team->global_lock));
	gettimeofday(&monitor->start_tm, NULL);
}
