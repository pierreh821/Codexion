/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 18:18:49 by phenry            #+#    #+#             */
/*   Updated: 2026/07/07 02:30:44 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dongle.h"
#include "codexion.h"

#include <stdlib.h>

t_dongle	*create_dongle()
{
	t_dongle	*dongle;

	dongle = malloc(sizeof(t_dongle *));
	if (pthread_mutex_init(&(dongle->lock), NULL) != 0)
		error("Failed to create dongle's mutex");
	return (dongle);
}

void free_dongle(t_dongle	*dongle)
{
	pthread_mutex_destroy(&(dongle->lock));
	free(dongle);
}
