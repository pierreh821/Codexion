/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 21:16:51 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 00:04:12 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	test_heap(t_table *table, t_coder *coder)
{
	t_heap		*heap;
	t_waiter	*waiter;

	heap = init_heap(table);
	waiter = init_waiter(coder);
	int i = 0;
	while (i < heap->size)
	{
		printf("%d: %d\n", i, heap->items[i]->coder->id);
		i++;
	}
	heap_push(heap, waiter, fifo_cmp);
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
	free_table(table);
	if (reason == STOP_BURNOUT)
		return (1);
	return (0);
}
