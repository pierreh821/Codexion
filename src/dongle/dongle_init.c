/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 18:18:49 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 01:22:55 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	free_dongle(t_dongle *dongle)
{
	if (!dongle)
		return ;
	pthread_mutex_destroy(&dongle->lock);
	if (dongle->waitlist)
		free_heap(dongle->waitlist);
	free(dongle);
}

t_dongle	*init_dongle(t_table *table, int id)
{
	t_dongle	*dongle;

	dongle = ft_calloc(1, sizeof(t_dongle));
	if (!dongle)
		return (NULL);
	dongle->waitlist = init_heap(table);
	if (!dongle->waitlist)
		return (free_dongle(dongle), NULL);
	if (pthread_mutex_init(&dongle->lock, NULL) != 0)
		return (free_dongle(dongle), NULL);
	dongle->id = id;
	dongle->table = table;
	dongle->strategy = table->args->strategy;
	return (dongle);
}

void	free_dongle_set(t_dongle **dongle_set, int nb)
{
	int	i;

	i = 0;
	while (i < nb)
		free_dongle(dongle_set[i++]);
	free(dongle_set);
}

t_heap	*init_heap(t_table *table)
{
	t_heap	*heap;

	heap = ft_calloc(1, sizeof(t_heap));
	if (!heap)
		return (NULL);
	heap->capacity = table->args->number_of_coders;
	heap->items = ft_calloc(heap->capacity, sizeof(t_waiter *));
	if (!heap->items)
		return (free(heap), NULL);
	heap->size = 0;
	return (heap);
}

void	free_heap(t_heap *heap)
{
	if (!heap)
		return ;
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
		team->dongle_set[id] = init_dongle(table, id);
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
