/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 21:16:51 by phenry            #+#    #+#             */
/*   Updated: 2026/07/06 17:23:28 by phenry           ###   ########.fr       */
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

	args = clean_args(argc, argv);
	coders = create_coders(args->number_of_coders);
	wait_coders(coders);

	free_coders(coders);
	return (0);
}
