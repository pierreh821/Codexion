/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 02:59:20 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 03:10:49 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"
#include <stdlib.h>

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

int	init_waiter(t_waiter *waiter, t_coder *coder, long priority)
{
	waiter->coder = coder;
	waiter->priority = priority;
	waiter->chosen = 0;
	if (pthread_cond_init(&waiter->cond, NULL) != 0)
		return (0);
	return (1);
}
