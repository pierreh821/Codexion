/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_schedule.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 23:07:23 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 00:05:27 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

int	check_running_dongle(t_dongle *dongle)
{
	return (is_running(dongle->table));
}

int	edf_cmp(t_waiter *a, t_waiter *b)
{
	return (a->priority < b->priority); // for now, change later
}

int	fifo_cmp(t_waiter *a, t_waiter *b)
{
	return (a->priority < b->priority);
}
