/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 04:08:09 by phenry            #+#    #+#             */
/*   Updated: 2026/07/11 16:21:28 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODERS_H
# define CODERS_H
# include <pthread.h>
# include <sys/time.h>
# include "dongle.h"
# include "table.h"

typedef struct s_coder
{
	int				id;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	pthread_t		thread_id;
	pthread_cond_t	*run;
	pthread_mutex_t	*run_lock;
	int				*run_signal;
	struct timeval	*time;
	t_table			*table;
}	t_coder;

typedef struct s_team
{
	t_coder			**coders_list;
	int				nb;
	pthread_cond_t	run;
	pthread_mutex_t	run_lock;
	int				run_signal;
	t_dongle		**dongle_set;
	void			*(*work)(void *);
}	t_team;

#endif
