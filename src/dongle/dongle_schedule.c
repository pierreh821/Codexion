/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_schedule.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 23:07:23 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 00:32:07 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

int	check_running_dongle(t_dongle *dongle)
{
	return (is_running(dongle->table));
}

int	edf_cmp(t_waiter *a, t_waiter *b)
{
	long	deadline_a;
	long	deadline_b;

	deadline_a = a->coder->start + a->coder->table->args->time_to_burnout;
	deadline_b = b->coder->start + b->coder->table->args->time_to_burnout;
	if (deadline_a != deadline_b)
		return (deadline_a < deadline_b);
	return (a->coder->id < b->coder->id);
}

int	fifo_cmp(t_waiter *a, t_waiter *b)
{
	return (a->priority < b->priority);
}
