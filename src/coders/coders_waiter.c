/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_waiter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 11:27:32 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 00:03:55 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

t_waiter	*init_waiter(t_coder *coder)
{
	t_waiter	*waiter;

	waiter = ft_calloc(1, sizeof(t_coder));
	if (!waiter)
		return (NULL);
	if (pthread_cond_init(&waiter->cond, NULL) != 0)
		return (free(waiter), NULL);
	waiter->coder = coder;
	waiter->chosen = 0;
	waiter->priority = 0;
	return (waiter);
}
