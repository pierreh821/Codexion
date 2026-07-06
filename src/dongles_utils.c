/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 18:18:49 by phenry            #+#    #+#             */
/*   Updated: 2026/07/06 18:41:27 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dongles.h"
#include "codexion.h"

#include <stdlib.h>

t_dongles	*alloc_dongles(int nb)
{
	t_dongles		*dongles;

	dongles = malloc(sizeof(t_dongles));
	if (!dongles)
		error("Cannot allocate memory for dongles");
	dongles->dongles_list = malloc(sizeof(t_dongle *) * nb);
	if (!dongles->dongles_list)
		error("Cannot allocate memory for dongles list");
	return (dongles);
}

t_dongles	*create_dongles(int nb)
{
	t_dongles	*dongles;
	t_dongle	*dongle;
	int			i;

	dongles = alloc_dongles(nb);
	i = 0;
	while (i < nb)
	{
		dongle = dongles->dongles_list[i];
		dongle = malloc(sizeof(t_dongle));
		if (pthread_mutex_init(&(dongle->lock), NULL) != 0)
			error("Failed to create dongle's mutex");
		i++;
	}
	dongles->nb = nb;
	return (dongles);
}

void	free_dongles(t_dongles *dongles)
{
	int	i;

	i = 0;
	while (i < dongles->nb)
	{
		pthread_mutex_destroy(&(dongles->dongles_list[i]->lock));
		free(dongles->dongles_list[i]);
		i++;
	}
	free(dongles->dongles_list);
	free(dongles);
}
