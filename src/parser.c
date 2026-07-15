/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 21:19:48 by phenry            #+#    #+#             */
/*   Updated: 2026/07/15 15:16:17 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

int	args_validator(t_args *args)
{
	if (args->number_of_coders >= 1024)
	{
		printf("Number of coders exceeding max (1024)");
		return (0);
	}
	if (strcmp(args->schedule, "fifo") != 0
		&& strcmp(args->schedule, "edf") != 0)
	{
		printf("Invalid schedule, must be 'fifo' or 'edf'");
		return (0);
	}
	return (1);
}

t_args	*clean_args(int argc, char *argv[])
{
	t_args	*args;

	if (argc != 9)
	{
		printf("Invalid number of arguments");
		return (NULL);
	}
	args = ft_calloc(1, sizeof(t_args));
	args->number_of_coders = atoi(argv[1]);
	args->time_to_burnout = atoi(argv[2]);
	args->time_to_compile = atoi(argv[3]) * 1000;
	args->time_to_debug = atoi(argv[4]) * 1000;
	args->time_to_refactor = atoi(argv[5]) * 1000;
	args->number_of_compiles_required = atoi(argv[6]);
	args->dongle_cooldown = atoi(argv[7]);
	args->schedule = argv[8];
	if (!args_validator(args))
		return (NULL);
	return (args);
}

