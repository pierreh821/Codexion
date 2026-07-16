/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dev_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 03:12:03 by phenry            #+#    #+#             */
/*   Updated: 2026/07/16 19:36:14 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	describe_tm(t_team *team)
{
	int		i;
	t_coder	*coder;

	i = 0;
	while (i < team->nb)
	{
		coder = team->coders_list[i];
		printf("Coder %d %p\n", coder->id, (void *)coder);
		printf("Left dongle: %d %p\n", coder->left_dongle->id,
			(void *)&(coder->left_dongle->waitlist_lock));
		printf("Right dongle: %d %p\n\n", coder->right_dongle->id,
			(void *)&(coder->right_dongle->waitlist_lock));
		i++;
	}
}
