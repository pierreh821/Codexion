/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 02:06:50 by pierre            #+#    #+#             */
/*   Updated: 2026/07/18 00:07:29 by phenry           ###   ########.fr       */
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
	int		id;
	t_table	*table;
	t_heap	*waitlist;
}	t_dongle;

#endif
