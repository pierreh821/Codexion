/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 21:50:24 by phenry            #+#    #+#             */
/*   Updated: 2026/07/05 22:02:35 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h> // for foo, remove later
#include <stdlib.h>
#include <sys/time.h>

#include <stdio.h>
#include "codexion.h"
#include "coder.h"
#include "coders.h"

void	*foo(void *arg)
{
	t_coder			*coder;
	struct timeval	tv;

	coder = (t_coder *)arg;
	pthread_mutex_lock(coder->lock);
	gettimeofday(&tv, NULL);
	printf("%ld: Coder %d obtained mutex ! Releasing it in 1s..\n",
		tv.tv_sec, coder->id);
	usleep(1000000);
	pthread_mutex_unlock(coder->lock);
	gettimeofday(&tv, NULL);
	printf("%ld: Coder %d released mutex\n\n", tv.tv_sec, coder->id);
	return (NULL);
}

t_coders	*alloc_coders(int nb)
{
	t_coders		*coders;

	coders = malloc(sizeof(t_coders));
	if (!coders)
		error("Cannot allocate memory for coders");
	coders->coders_list = malloc(sizeof(t_coder *) * nb);
	if (!coders->coders_list)
		error("Cannot allocate memory for coders list");
	return (coders);
}

t_coders	*create_coders(int nb)
{
	t_coders		*coders;
	int				id;
	int				res;

	id = 0;
	coders = alloc_coders(nb);
	if (pthread_mutex_init(&(coders->lock), NULL) != 0)
		error("Failed to init mutex");
	pthread_mutex_lock(&(coders->lock));
	while (id < nb)
	{
		coders->coders_list[id] = malloc(sizeof(t_coder));
		coders->coders_list[id]->id = id + 1;
		coders->coders_list[id]->lock = &(coders->lock);
		res = pthread_create(&coders->coders_list[id]->thread_id, NULL, foo,
				coders->coders_list[id]);
		if (res != 0)
			error("error while creating thread");
		id++;
	}
	return (coders);
}

void	free_coders(t_coders *coders, int nb)
{
	int	id;

	id = 0;
	while (id < nb)
	{
		free(coders->coders_list[id]);
		id++;
	}
	free(coders->coders_list);
	free(coders);
}
