/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_waiter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 11:27:32 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 01:57:20 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

int	init_waiter(t_waiter *waiter, t_coder *coder, long priority)
{
	waiter->coder = coder;
	waiter->priority = priority;
	waiter->chosen = 0;
	if (pthread_cond_init(&waiter->cond, NULL) != 0)
		return (0);
	return (1);
}
