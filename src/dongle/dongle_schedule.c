/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_schedule.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 23:07:23 by phenry            #+#    #+#             */
/*   Updated: 2026/07/16 23:35:38 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

int	check_running_dongle(t_dongle *dongle)
{
	return (is_running(dongle->table));
}

int	edf(t_dongle *dongle)
{
	return (1);
}

int	fifo(t_dongle *dongle)
{
	t_coder	*selected;

	selected = dongle_waitlist_pop(dongle, 0);
	return (1);
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
	while (check_running_dongle(dongle))
		cycle(dongle);
	return (NULL);
}
