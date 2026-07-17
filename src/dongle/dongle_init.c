/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 18:18:49 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 00:27:38 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

t_dongle	*create_dongle(t_table *table, int id)
{
	t_dongle	*dongle;

	dongle = ft_calloc(1, sizeof(t_dongle));
	if (!dongle)
		return (NULL);
	dongle->waitlist = init_heap(table);
	if (!dongle->waitlist)
		return (free(dongle), NULL);
	dongle->id = id;
	dongle->table = table;
	return (dongle);
}

void	free_dongles(t_dongle **dongle_set, int nb)
{
	int	i;

	i = 0;
	while (i < nb)
	{
		free_heap(dongle_set[i]->waitlist);
		free(dongle_set[i]);
		i++;
	}
	free(dongle_set);
}

t_heap	*init_heap(t_table *table)
{
	t_heap	*heap;

	heap = ft_calloc(1, sizeof(t_heap));
	if (!heap)
		return (NULL);
	heap->capacity = table->args->number_of_coders;
	heap->items = ft_calloc(heap->capacity, sizeof(t_waiter));
	if (!heap->items)
		return (free(heap), NULL);
	heap->size = 0;
	return (heap);
}

void	free_heap(t_heap *heap)
{
	int	i;

	i = 0;
	while (i < heap->size)
	{
		free(heap->items[i]);
		i++;
	}
	free(heap->items);
	free(heap);
}

int	assign_dongles(t_table *table)
{
	int		id;
	int		nb;
	t_team	*team;

	team = table->team;
	nb = team->nb;
	id = 0;
	while (id < nb)
	{
		team->dongle_set[id] = create_dongle(table, id);
		if (!team->dongle_set[id])
			return (1);
		id++;
	}
	id = 0;
	while (id < nb)
	{
		team->coders_list[id]->left_dongle = team->dongle_set[id];
		team->coders_list[id]->right_dongle = team->dongle_set[(id + 1) % nb];
		id++;
	}
	return (0);
}
