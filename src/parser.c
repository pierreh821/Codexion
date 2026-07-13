/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 21:19:48 by phenry            #+#    #+#             */
/*   Updated: 2026/07/13 18:02:01 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

t_args	*clean_args(int argc, char *argv[])
{
	t_args	*args;

	if (argc != 9)
		error("Invalid number of arguments");
	args = malloc(sizeof(t_args));
	args->number_of_coders = atoi(argv[1]);
	args->time_to_burnout = atoi(argv[2]);
	args->time_to_compile = atoi(argv[3]) * 1000;
	args->time_to_debug = atoi(argv[4]) * 1000;
	args->time_to_refactor = atoi(argv[5]) * 1000;
	args->number_of_compiles_required = atoi(argv[6]);
	args->dongle_cooldown = atoi(argv[7]);
	args->schedule = argv[8];
	args_validator(args);
	return (args);
}

void	args_validator(t_args *args)
{
	if (args->number_of_coders >= 1024)
		error("Number of coders exciding max (1024)");
	if (strcmp(args->schedule, "fifo") != 0
		&& strcmp(args->schedule, "edf") != 0)
		error("Invalid schedule, must be 'fifo' or 'edf'");
}
