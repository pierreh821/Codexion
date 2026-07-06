/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 21:16:51 by phenry            #+#    #+#             */
/*   Updated: 2026/07/06 19:01:07 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h> // for dev, remove later
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#include "codexion.h"
#include "coders.h"
#include "args.h"
#include "dongles.h"

void	error(char *message)
{
	printf("%s", message);
	exit(0);
}

int	main(int argc, char *argv[])
{
	t_team			*coders;
	t_dongle_set	*dongles;
	t_args			*args;

	args = clean_args(argc, argv);
	coders = create_coders(args->number_of_coders);
	dongles = create_dongles(args->number_of_coders);

	wait_coders(coders);
	free_coders(coders);
	free_dongles(dongles);
	free(args);
	return (0);
}
