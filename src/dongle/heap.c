/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 03:01:22 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 03:01:53 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	swap(t_heap *heap, int a, int b)
{
	t_waiter	*swap;

	swap = heap->items[a];
	heap->items[a] = heap->items[b];
	heap->items[b] = swap;
}

int	heap_push(t_heap *heap, t_waiter *waiter,
	int (*cmp)(t_waiter *, t_waiter *))
{
	int	i;
	int	parent;

	if (heap->size >= heap->capacity)
		return (0);
	i = heap->size;
	heap->items[i] = waiter;
	heap->size++;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (!cmp(heap->items[i], heap->items[parent]))
			break ;
		swap(heap, i, parent);
		i = parent;
	}
	return (1);
}

int	heap_smallest_child(t_heap *heap, int i, int (*cmp)(t_waiter *, t_waiter *))
{
	int	left;
	int	right;
	int	smallest;

	left = 2 * i + 1;
	right = 2 * i + 2;
	smallest = i;
	if (left < heap->size && cmp(heap->items[left], heap->items[smallest]))
		smallest = left;
	if (right < heap->size && cmp(heap->items[right], heap->items[smallest]))
		smallest = right;
	return (smallest);
}

t_waiter	*heap_pop(t_heap *heap, int (*cmp)(t_waiter *, t_waiter *))
{
	t_waiter	*removed;
	int			i;
	int			smallest;

	if (heap->size == 0)
		return (NULL);
	removed = heap->items[0];
	heap->size--;
	heap->items[0] = heap->items[heap->size];
	i = 0;
	smallest = heap_smallest_child(heap, i, cmp);
	while (smallest != i)
	{
		swap(heap, i, smallest);
		i = smallest;
		smallest = heap_smallest_child(heap, i, cmp);
	}
	return (removed);
}
