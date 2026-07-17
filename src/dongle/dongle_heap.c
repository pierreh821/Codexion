/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_heap.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 00:45:24 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 00:09:38 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

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
		swap(heap, i + 1, i);
		i--;
	}
	return (1);
}

t_waiter	*heap_pop(t_heap *heap, int (*cmp)(t_waiter *, t_waiter *))
{
	t_waiter	*removed;
	int			i;
	int			left;
	int			right;
	int			smallest;

	if (heap->size == 0)
		return (NULL);
	removed = heap->items[0];
	heap->size--;
	heap->items[0] = heap->items[heap->size];
	i = 0;
	while (1)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		smallest = i;
		if (left < heap->size && cmp(heap->items[left], heap->items[smallest]))
			smallest = left;
		if (right < heap->size && cmp(heap->items[right],
				heap->items[smallest]))
			smallest = right;
		if (smallest == i)
			break ;
		swap(heap, i, smallest);
		i = smallest;
	}
	return (removed);
}
