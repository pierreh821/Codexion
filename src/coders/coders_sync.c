/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_sync.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 22:50:20 by phenry            #+#    #+#             */
/*   Updated: 2026/07/16 23:32:45 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	wait_for_start(t_coder *coder)
{
	pthread_mutex_lock(coder->run_lock);
	while (*(coder->run_signal) == 0)
	{
		pthread_cond_wait(coder->run, coder->run_lock);
	}
	pthread_mutex_unlock(coder->run_lock);
}

void	wait_team(t_team *team)
{
	int	id;

	id = 0;
	while (id < team->nb)
	{
		pthread_join(team->coders_list[id]->thread_id, NULL);
		id++;
	}
}

void	dongle_order(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		coder->first = coder->left_dongle;
		coder->second = coder->right_dongle;
	}
	else
	{
		coder->first = coder->right_dongle;
		coder->second = coder->left_dongle;
	}
}	

int	check_running_coder(t_coder *coder)
{
	return (is_running(coder->table));
}
