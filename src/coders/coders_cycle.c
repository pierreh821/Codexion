/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_cycle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 02:11:05 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 23:05:26 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

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

int	take_dongles(t_coder *coder)
{
	if (!take_dongle(coder->first, coder))
		return (0);
	logger_write(coder, "has taken a dongle");
	if (coder->first == coder->second)
		return (1);
	if (!take_dongle(coder->second, coder))
	{
		release_dongle(coder->first);
		return (0);
	}
	logger_write(coder, "has taken a dongle");
	return (1);
}

int	work_cycle(t_coder *coder)
{
	if (!is_running(coder->table))
		return (0);
	if (!take_dongles(coder))
		return (0);
	if (!is_running(coder->table))
	{
		if (coder->second != coder->first)
			release_dongle(coder->second);
		release_dongle(coder->first);
		return (0);
	}
	compile(coder);
	debug(coder);
	refactor(coder);
	return (1);
}
