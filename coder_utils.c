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

#include <stdio.h>
#include "codexion.h"
#include "coder.h"

void	*foo(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (1)
	{
		printf("Coder %d testing\n", coder->id);
		usleep(500000);
	}
	return (NULL);
}

t_coder	**create_coders(int nb)
{
	t_coder	**coders;
	int		id;
	int		res;

	id = 0;
	res = 0;
	coders = malloc(sizeof(t_coder *) * nb);
	if (!coders)
		error("Malloc failed");
	while (id < nb)
	{
		coders[id] = malloc(sizeof(t_coder));
		coders[id]->id = id + 1;
		res = pthread_create(&coders[id]->thread, NULL, foo, coders[id]);
		if (res != 0)
			error("error while creating thread");
		id++;
	}
	return (coders);
}

void	free_coders(t_coder **coders, int nb)
{
	int	id;

	id = 0;
	while (id < nb)
	{
		free(coders[id]);
		id++;
	}
	free(coders);
}
