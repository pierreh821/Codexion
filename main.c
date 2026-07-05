/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 21:16:51 by phenry            #+#    #+#             */
/*   Updated: 2026/07/05 20:05:34 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "models/coder.h"
#include "models/args.h"

void	*foo(void *arg)
{
	t_coder	*coder = (t_coder *)arg;

	while (1)
	{
		printf("Coder %d testing\n", coder->id);
		usleep(500000);
	}
	return NULL;
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
	{
		printf("Malloc failed");
		exit(0);
	}
	while (id < nb)
	{
		coders[id] = malloc(sizeof(t_coder));
		coders[id]->id = id + 1;
		res = pthread_create(&coders[id]->thread, NULL, foo, coders[id]);
		if (res != 0)
		{
			printf("error while creating thread");
			exit(0);
		}
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

t_args	*clean_args(int argc, char *argv[])
{
	t_args	*args;

	if (argc != 9)
	{
		printf("Invalid arguments");
		exit(0);
	}
	args = malloc(sizeof(t_args));
	args->number_of_coders = atoi(argv[1]);
	args->time_to_burnout = atoi(argv[2]);
	args->time_to_compile = atoi(argv[3]);
	args->time_to_debug = atoi(argv[4]);
	args->time_to_refactor = atoi(argv[5]);
	args->number_of_compiles_required = atoi(argv[6]);
	args->dongle_cooldown = atoi(argv[7]);
	args->schedule = argv[8];
	return (args);
}

int	main(int argc, char *argv[])
{
	t_coder	**coders;
	t_args	*args;

	args = clean_args(argc, argv);
	coders = create_coders(args->number_of_coders);
	usleep(3000000);
	free_coders(coders, args->number_of_coders);
	return (0);
}
