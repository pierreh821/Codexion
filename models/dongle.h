/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 02:06:50 by pierre            #+#    #+#             */
/*   Updated: 2026/07/19 23:53:04 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DONGLE_H
# define DONGLE_H

# include <pthread.h>
# include "table.h"

typedef struct s_coder		t_coder;

typedef enum e_strategy
{
	FIFO,
	EDF,
}	t_strategy;

typedef struct s_waiter
{
	t_coder			*coder;
	long			priority;
	pthread_cond_t	cond;
	int				chosen;
}	t_waiter;

typedef struct s_heap
{
	t_waiter	**items;
	int			size;
	int			capacity;
}	t_heap;

typedef struct s_dongle
{
	int				id;
	t_table			*table;
	t_heap			*waitlist;
	pthread_mutex_t	lock;
	long			next_ticket;
	int				in_use;
	long			released;
}	t_dongle;

t_dongle	*init_dongle(t_table *table, int id);
void		free_dongle(t_dongle *dongle);
void		free_dongle_set(t_dongle **dongle_set, int nb);
int			assign_dongles(t_table *table);

int			queue_dongle(t_dongle *dongle, t_coder *coder, t_waiter *waiter);
int			try_fast_dongle(t_dongle *dongle, t_coder *coder);
int			take_dongle(t_dongle *dongle, t_coder *coder);
void		release_dongle(t_dongle *dongle);

long		compute_priority(t_dongle *dongle, t_coder *coder);
int			waiter_cmp(t_waiter *a, t_waiter *b);

t_heap		*init_heap(t_table *table);
void		free_heap(t_heap *heap);
int			init_waiter(t_waiter *waiter, t_coder *coder, long priority);

void		swap(t_heap *heap, int a, int b);
int			heap_push(t_heap *heap, t_waiter *waiter,
				int (*cmp)(t_waiter *, t_waiter *));
int			heap_smallest_child(t_heap *heap, int i,
				int (*cmp)(t_waiter *, t_waiter *));
t_waiter	*heap_pop(t_heap *heap, int (*cmp)(t_waiter *, t_waiter *));

#endif
