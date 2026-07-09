/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 04:08:09 by phenry            #+#    #+#             */
/*   Updated: 2026/07/09 18:29:04 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODERS_H
# define CODERS_H
# include <pthread.h>
# include <sys/time.h>
# include "dongle.h"

typedef struct s_coder
{
	int				id;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	pthread_t		thread_id;
	pthread_mutex_t	*global_lock;
	struct timeval	*time;
}	t_coder;

typedef struct s_team
{
	t_coder			**coders_list;
	pthread_mutex_t	global_lock;
	int				nb;
	t_dongle		**dongle_set;
}	t_team;

#endif
