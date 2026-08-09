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

void	free_coders_list(t_coder **coders_list, int nb)
{
	int	id;

	id = 0;
	while (id < nb)
	{
		if (coders_list[id])
		{
			if (coders_list[id]->is_mutex_init)
				pthread_mutex_destroy(&coders_list[id]->task_lock);
			free(coders_list[id]);
		}
		id++;
	}
	free(coders_list);
}

void	free_team(t_team *team)
{
	if (!team)
		return ;
	if (team->coders_list)
		free_coders_list(team->coders_list, team->nb);
	if (team->dongle_set)
		free_dongle_set(team->dongle_set, team->nb);
	if (team->is_cond_init)
	{
		pthread_mutex_destroy(&(team->run_lock));
		pthread_cond_destroy(&(team->run));
	}
	free(team);
	team = NULL;
}
