/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 17:36:32 by phenry            #+#    #+#             */
/*   Updated: 2026/07/11 19:18:47 by phenry           ###   ########.fr       */
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

void	*work(void *inp)
{
	t_coder	*coder;

	coder = (t_coder *)inp;
	wait_for_start(coder);
	return (NULL);
}
