/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 02:22:32 by phenry            #+#    #+#             */
/*   Updated: 2026/07/19 23:58:18 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int	input_validator(int argc, char *argv[])
{
	if (argc != 9)
		return ((void)fprintf(stderr, "Invalid number of arguments\n"), 0);
	if (!ft_isnumber(argv[1]))
		return ((void)fprintf(stderr, "number_of_coders must be a \
non-negative integer and 9 digits max\n"), 0);
	if (!ft_isnumber(argv[2]))
		return ((void)fprintf(stderr, "time_to_burnout must be a \
non-negative integer and 9 digits max\n"), 0);
	if (!ft_isnumber(argv[3]))
		return ((void)fprintf(stderr, "time_to_compile must be a \
non-negative integer and 9 digits max\n"), 0);
	if (!ft_isnumber(argv[4]))
		return ((void)fprintf(stderr, "time_to_debug must be a \
non-negative integer and 9 digits max\n"), 0);
	if (!ft_isnumber(argv[5]))
		return ((void)fprintf(stderr, "time_to_refactor must be a \
non-negative integer and 9 digits max\n"), 0);
	if (!ft_isnumber(argv[6]))
		return ((void)fprintf(stderr, "number_of_compiles must be a \
non-negative integer and 9 digits max\n"), 0);
	if (!ft_isnumber(argv[7]))
		return ((void)fprintf(stderr, "dongle_cooldown must be a \
non-negative integer and 9 digits max\n"), 0);
	return (1);
}

int	args_validator(t_args *args)
{
	if (args->number_of_coders < 1)
	{
		fprintf(stderr, "number_of_coders must be at least 1\n");
		return (0);
	}
	if (args->number_of_coders > 300)
	{
		fprintf(stderr, "number_of_coder must be less or equal to 300\n");
		return (0);
	}
	return (1);
}

t_args	*clean_args(int argc, char *argv[])
{
	t_args	*args;

	if (!input_validator(argc, argv))
		return (NULL);
	args = ft_calloc(1, sizeof(t_args));
	args->number_of_coders = atoi(argv[1]);
	args->time_to_burnout = atoi(argv[2]);
	args->time_to_compile = atoi(argv[3]) * 1000;
	args->time_to_debug = atoi(argv[4]) * 1000;
	args->time_to_refactor = atoi(argv[5]) * 1000;
	args->number_of_compiles_required = atoi(argv[6]);
	args->dongle_cooldown = atoi(argv[7]);
	if (!set_schedule(args, argv[8]))
		return (free(args), NULL);
	if (!args_validator(args))
		return (free(args), NULL);
	return (args);
}

int	set_schedule(t_args *args, char *input)
{
	if (strcmp(input, "fifo") == 0)
		args->strategy = FIFO;
	else if (strcmp(input, "edf") == 0)
		args->strategy = EDF;
	else
	{
		printf("scheduler must be fifo or edf\n");
		return (0);
	}
	return (1);
}
