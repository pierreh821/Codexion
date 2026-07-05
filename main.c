/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 21:16:51 by phenry            #+#    #+#             */
/*   Updated: 2026/07/05 17:41:23 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "models/coder.h"

int g_running = 1;

void *test_function(void *arg)
{
	t_coder *coder = (t_coder *)arg;

	while (g_running)
	{
		printf("Coder %d is testing\n", coder->id);
		usleep(1000000);
	}
	printf("Coder %d finished testing\n", coder->id);
	return NULL;
}

void	schedule(int number_of_coders)
{
	int		id;
	int		res;
	t_coder	**coders;

	// create coders
	coders = malloc(sizeof(t_coder *) * number_of_coders);
	if (!coders)
		return;

	id = 0;
	while (id < number_of_coders)
	{
		coders[id] = malloc(sizeof(t_coder));
		coders[id]->id = id + 1;

		res = pthread_create(&coders[id]->thread, NULL, test_function, coders[id]);
		if (res != 0)
			perror("error while creating thread");
		id++;
	}

	id = 0;
	while (id < 5)
	{
		printf("\n%ds elapsed\n\n", id);
		usleep(1000000);
		id++;
	}
	g_running = 0;

	// wait end of threads
	id = 0;
	while (id < number_of_coders)
	{
		pthread_join(coders[id]->thread, NULL);
		id++;
	}

	// free
	id = 0;
	while (id < number_of_coders)
	{
		free(coders[id]);
		id++;
	}
	free(coders);
}

int main()
{
	schedule(3);
	return (0);
}
