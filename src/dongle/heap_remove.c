/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_remove.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/02 23:32:02 by phenry            #+#    #+#             */
/*   Updated: 2026/08/02 23:33:35 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

int	heap_find_index(t_heap *heap, t_waiter *waiter)
{
	int	i;

	i = 0;
	while (i < heap->size && heap->items[i] != waiter)
		i++;
	return (i);
}

void	heap_reheapify(t_heap *heap, int i, int (*cmp)(t_waiter *, t_waiter *))
{
	int	parent;
	int	smallest;

	parent = (i - 1) / 2;
	while (i > 0 && cmp(heap->items[i], heap->items[parent]))
	{
		swap(heap, i, parent);
		i = parent;
		parent = (i - 1) / 2;
	}
	smallest = heap_smallest_child(heap, i, cmp);
	while (smallest != i)
	{
		swap(heap, i, smallest);
		i = smallest;
		smallest = heap_smallest_child(heap, i, cmp);
	}
}

void	heap_remove_waiter(t_heap *heap, t_waiter *waiter,
	int (*cmp)(t_waiter *, t_waiter *))
{
	int	i;

	i = heap_find_index(heap, waiter);
	if (i == heap->size)
		return ;
	heap->size--;
	heap->items[i] = heap->items[heap->size];
	heap->items[heap->size] = NULL;
	if (i != heap->size)
		heap_reheapify(heap, i, cmp);
}
