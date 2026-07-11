/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 21:50:24 by phenry            #+#    #+#             */
/*   Updated: 2026/07/05 22:02:35 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	team_resume(t_team *team)
{
	pthread_mutex_lock(&(team->run_lock));
	team->run_signal = 1;
	pthread_cond_broadcast(&(team->run));
	pthread_mutex_unlock(&(team->run_lock));
}

void	team_pause(t_team *team)
{
	pthread_mutex_lock(&(team->run_lock));
	team->run_signal = 0;
	pthread_cond_broadcast(&(team->run));
	pthread_mutex_unlock(&(team->run_lock));
}

void	free_team(t_team *team)
{
	int	id;

	id = 0;
	while (id < team->nb)
	{
		pthread_join(team->coders_list[id]->thread_id, NULL);
		free_dongle(team->coders_list[id]->left_dongle);
		free(team->coders_list[id]);
		id++;
	}
	free(team->coders_list);
	free(team->dongle_set);
	pthread_mutex_destroy(&(team->run_lock));
	pthread_cond_destroy(&(team->run));
	free(team);
}
