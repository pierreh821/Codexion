/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 18:18:49 by phenry            #+#    #+#             */
/*   Updated: 2026/07/11 19:19:10 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

t_dongle	*create_dongle(int id)
{
	t_dongle	*dongle;

	dongle = malloc(sizeof(t_dongle));
	if (pthread_mutex_init(&(dongle->lock), NULL) != 0)
		error("Failed to create dongle's mutex");
	dongle->id = id;
	return (dongle);
}

void	free_dongle(t_dongle *dongle)
{
	pthread_mutex_destroy(&(dongle->lock));
	free(dongle);
}
