/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 19:47:10 by phenry            #+#    #+#             */
/*   Updated: 2026/08/03 15:10:16 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGS_H
# define ARGS_H

# include "dongle.h"

# define ERR_NB_ARGS "Invalid number of arguments\n"
# define ERR_NUMBER "Argument must be a non-negative integer and 9 digits max\n"

typedef struct s_args
{
	long		number_of_coders;
	long		time_to_burnout;
	long		time_to_compile;
	long		time_to_debug;
	long		time_to_refactor;
	long		number_of_compiles_required;
	long		dongle_cooldown;
	t_strategy	strategy;
}	t_args;

t_args	*clean_args(int argc, char *argv[]);
int		set_schedule(t_args *args, char *input);

#endif
