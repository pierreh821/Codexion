/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 17:47:20 by phenry            #+#    #+#             */
/*   Updated: 2026/07/06 18:50:49 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DONGLES_H
# include <pthread.h>

typedef struct s_dongle
{
	pthread_mutex_t	lock;
}	t_dongle;

typedef struct s_dongle_set
{
	t_dongle	**dongles_list;
	int			nb;
}	t_dongle_set;

#endif
