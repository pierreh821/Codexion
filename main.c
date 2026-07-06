/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 21:16:51 by phenry            #+#    #+#             */
/*   Updated: 2026/07/06 16:52:07 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h> // for dev, remove later
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#include "codexion.h"
#include "coder.h"
#include "coders.h"
#include "args.h"

void	error(char *message)
{
	printf("%s", message);
	exit(0);
}

int	main(int argc, char *argv[])
{
	t_coders		*coders;
	t_args			*args;
	struct timeval	tv;

	args = clean_args(argc, argv);
	gettimeofday(&tv, NULL);
	printf("%ld: Creating coders and locking mutex for 3s...\n", tv.tv_sec);
	coders = create_coders(args->number_of_coders);
	usleep(3000000);
	gettimeofday(&tv, NULL);
	printf("%ld: Countdown over, releasing mutex\n\n", tv.tv_sec);
	pthread_mutex_unlock(&(coders->lock));

	int id = 0;
	while (id < args->number_of_coders)
	{
		pthread_join(coders->coders_list[id]->thread_id, NULL);
		id++;
	}

	gettimeofday(&tv, NULL);
	printf("%ld: All threads ended\n", tv.tv_sec);

	free_coders(coders, args->number_of_coders);
	return (0);
}
