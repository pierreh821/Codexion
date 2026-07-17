/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 21:16:51 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 00:15:18 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	test_heap(t_table *table)
{
	t_heap		*heap;
	t_waiter	*waiter;

	heap = init_heap(table);
	waiter = init_waiter(table->team->coders_list[0]);
	int i = 0;
	printf("add 0 coder\n");
	while (i < heap->size)
	{
		printf("%d: %d\n", i, heap->items[i]->coder->id);
		i++;
	}
	heap_push(heap, waiter, fifo_cmp);
	printf("add 1 coder\n");
	i = 0;
	while (i < heap->size)
	{
		printf("%d: %d\n", i, heap->items[i]->coder->id);
		i++;
	}
	waiter = init_waiter(table->team->coders_list[1]);
	heap_push(heap, waiter, fifo_cmp);
	printf("add 1 coder\n");
	i = 0;
	while (i < heap->size)
	{
		printf("%d: %d\n", i, heap->items[i]->coder->id);
		i++;
	}
	heap_pop(heap, fifo_cmp);
	printf("pop 1 coder\n");
	i = 0;
	while (i < heap->size)
	{
		printf("%d: %d\n", i, heap->items[i]->coder->id);
		i++;
	}
}

int	main(int argc, char *argv[])
{
	t_table			*table;
	t_stop_reason	reason;

	table = init_table(argc, argv, &work);
	if (!table)
		return (1);
	team_start(table);
	join_table(table);
	reason = table->status->reason;
	test_heap(table);
	free_table(table);
	if (reason == STOP_BURNOUT)
		return (1);
	return (0);
}
