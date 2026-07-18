/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_clean.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 03:22:19 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 15:43:29 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"
#include <stdlib.h>

void	free_team(t_team *team)
{
	int	id;

	if (!team)
		return ;
	id = 0;
	if (team->coders_list)
	{
		while (id < team->nb)
		{
			if (team->coders_list[id])
			{
				pthread_mutex_destroy(&team->coders_list[id]->task_lock);
				free(team->coders_list[id]);
			}
			id++;
		}
		free(team->coders_list);
	}
	if (team->dongle_set)
		free_dongle_set(team->dongle_set, team->nb);
	pthread_mutex_destroy(&(team->run_lock));
	pthread_cond_destroy(&(team->run));
	free(team);
}
