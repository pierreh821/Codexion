/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dev_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 03:12:03 by phenry            #+#    #+#             */
/*   Updated: 2026/07/07 03:16:58 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"
#include <stdio.h>
#include <stdlib.h>

void	error(char *message)
{
	printf("%s", message);
	exit(0);
}

void	*foo(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	printf("Coder %d testing\n", coder->id);
	return (NULL);
}
