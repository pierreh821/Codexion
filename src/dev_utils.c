/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dev_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 03:12:03 by phenry            #+#    #+#             */
/*   Updated: 2026/07/11 23:43:02 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

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

void	describe_tm(t_team *team)
{
	int		i;
	t_coder	*coder;

	i = 0;
	while (i < team->nb)
	{
		coder = team->coders_list[i];
		printf("Coder %d\n", coder->id);
		printf("Left dongle: %d %p\n", coder->left_dongle->id,
			(void *)&(coder->left_dongle->lock));
		printf("Right dongle: %d %p\n\n", coder->right_dongle->id,
			(void *)&(coder->right_dongle->lock));
		i++;
	}
}
