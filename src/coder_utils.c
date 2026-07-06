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
#include "coders.h"

void	*foo(void *arg)
{
	t_coder			*coder;

	coder = (t_coder *)arg;
	printf("Coder %d testing\n", coder->id);
	return (NULL);
}

t_team	*alloc_coders(int nb)
{
	t_team		*coders;

	coders = malloc(sizeof(t_team));
	if (!coders)
		error("Cannot allocate memory for coders");
	coders->coders_list = malloc(sizeof(t_coder *) * nb);
	if (!coders->coders_list)
		error("Cannot allocate memory for coders list");
	return (coders);
}

t_team	*create_coders(int nb)
{
	t_team		*coders;
	int				id;
	int				res;

	id = 0;
	coders = alloc_coders(nb);
	if (pthread_mutex_init(&(coders->global_lock), NULL) != 0)
		error("Failed to init mutex");
	pthread_mutex_lock(&(coders->global_lock));
	while (id < nb)
	{
		coders->coders_list[id] = malloc(sizeof(t_coder));
		coders->coders_list[id]->id = id + 1;
		coders->coders_list[id]->global_lock = coders->global_lock;
		res = pthread_create(&coders->coders_list[id]->thread_id, NULL, foo,
				coders->coders_list[id]);
		if (res != 0)
			error("Failed to create thread");
		id++;
	}
	coders->nb = nb;
	return (coders);
}

void	wait_coders(t_team *coders)
{
	int	id;

	id = 0;
	while (id < coders->nb)
	{
		pthread_join(coders->coders_list[id]->thread_id, NULL);
		id++;
	}
}

void	free_coders(t_team *coders)
{
	int	id;

	id = 0;
	while (id < coders->nb)
	{
		free(coders->coders_list[id]);
		id++;
	}
	free(coders->coders_list);
	pthread_mutex_destroy(&(coders->global_lock));
	free(coders);
}
