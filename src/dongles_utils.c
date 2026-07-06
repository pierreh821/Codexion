/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 18:18:49 by phenry            #+#    #+#             */
/*   Updated: 2026/07/06 19:06:28 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dongles.h"
#include "codexion.h"

#include <stdlib.h>

t_dongle_set	*alloc_dongles(int nb)
{
	t_dongle_set		*dongle_set;

	dongle_set = malloc(sizeof(t_dongle_set));
	if (!dongle_set)
		error("Cannot allocate memory for dongle_set");
	dongle_set->dongles_list = malloc(sizeof(t_dongle *) * nb);
	if (!dongle_set->dongles_list)
		error("Cannot allocate memory for dongles list");
	return (dongle_set);
}

t_dongle_set	*create_dongles(int nb)
{
	t_dongle_set	*dongle_set;
	int				i;

	dongle_set = alloc_dongles(nb);
	i = 0;
	while (i < nb)
	{
		dongle_set->dongles_list[i] = malloc(sizeof(t_dongle));
		if (pthread_mutex_init(&(dongle_set->dongles_list[i]->lock), NULL) != 0)
			error("Failed to create dongle's mutex");
		i++;
	}
	dongle_set->nb = nb;
	return (dongle_set);
}

void	free_dongles(t_dongle_set *dongles)
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
