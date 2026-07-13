/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 22:50:20 by phenry            #+#    #+#             */
/*   Updated: 2026/07/13 02:46:56 by phenry           ###   ########.fr       */
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

void	take_dongles(t_coder *coder)
{
	pthread_mutex_lock(&coder->first->lock);
	logger_write(coder, "has taken a dongle");
	pthread_mutex_lock(&coder->second->lock);
	logger_write(coder, "has taken a dongle");
}
