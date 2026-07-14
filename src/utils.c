/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 01:47:25 by phenry            #+#    #+#             */
/*   Updated: 2026/07/14 15:27:31 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	error(char *message)
{
	t_table		*table;
	static int	in_error = 0;

	if (in_error)
		return ;
	in_error = 1;
	if (message)
		fprintf(stderr, "Error: %s\n", message);
	table = get_table(NULL);
	if (table)
	{
		get_table((void *)-1);
		free_table(table);
	}
}
