/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 21:16:51 by phenry            #+#    #+#             */
/*   Updated: 2026/07/07 19:31:46 by phenry           ###   ########.fr       */
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
#include "dongle.h"

int	main(int argc, char *argv[])
{
	t_team			*team;
	t_args			*args;

	args = clean_args(argc, argv);
	team = create_coders(args->number_of_coders);
	wait_coders(team);
	describe_tm(team);
	free_coders(team);
	free(args);
	return (0);
}
