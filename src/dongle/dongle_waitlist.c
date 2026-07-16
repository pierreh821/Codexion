/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_waitlist.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 19:14:59 by phenry            #+#    #+#             */
/*   Updated: 2026/07/16 23:05:03 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	extend_waitlist_dongle(t_table *table, t_dongle *dongle, t_coder *coder)
{
	t_coder	**new_list;
	int		i;

	new_list = ft_calloc(dongle->waitlist_sz + 1, sizeof(t_coder *));
	if (!new_list)
		request_stop(table, STOP_FATAL, coder->id);
	i = 0;
	while (i < dongle->waitlist_sz)
	{
		new_list[i] = dongle->waitlist[i];
		i++;
	}
	new_list[i] = coder;
	free(dongle->waitlist);
	dongle->waitlist = new_list;
	dongle->waitlist_sz++;
}

void	take_dongles(t_coder *coder)
{
	pthread_mutex_lock(&coder->first->waitlist_lock);
	extend_waitlist_dongle(coder->table, coder->first, coder);
	pthread_mutex_unlock(&coder->first->waitlist_lock);
	logger_write(coder, "has taken a dongle");
	pthread_mutex_lock(&coder->second->waitlist_lock);
	extend_waitlist_dongle(coder->table, coder->second, coder);
	pthread_mutex_unlock(&coder->second->waitlist_lock);
	logger_write(coder, "has taken a dongle");
}

void	edf(t_dongle *dongle)
{
}

void	fifo(t_dongle *dongle)
{
}

void	*schedule(void *arg)
{
	void		(*cycle)(t_dongle *);
	t_dongle	*dongle;

	dongle = (t_dongle *)arg;
	if (dongle->table->args->strategy == FIFO)
		cycle = fifo;
	else if (dongle->table->args->strategy == EDF)
		cycle = edf;
	cycle(dongle);
}
