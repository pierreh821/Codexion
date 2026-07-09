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

#include "codexion.h"

// void	kill_coders(t_team *team)
// {

// }

void	wait_coders(t_team *team)
{
	int	id;

	id = 0;
	while (id < team->nb)
	{
		pthread_join(team->coders_list[id]->thread_id, NULL);
		id++;
	}
}

void	free_team(t_team *team)
{
	int	id;

	id = 0;
	while (id < team->nb)
	{
		free_dongle(team->coders_list[id]->left_dongle);
		free(team->coders_list[id]);
		id++;
	}
	free(team->coders_list);
	pthread_mutex_destroy(&(team->global_lock));
	free(team);
}
