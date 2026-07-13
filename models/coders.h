/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 04:08:09 by phenry            #+#    #+#             */
/*   Updated: 2026/07/13 19:22:49 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODERS_H
# define CODERS_H
# include <pthread.h>
# include <sys/time.h>
# include "dongle.h"
# include "table.h"

typedef enum e_status
{
	COMPILING,
	DEBUGGING,
	REFACTORING,
	SUSPEND
}	t_status;

typedef struct s_coder
{
	int				id;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	t_dongle		*first;
	t_dongle		*second;
	pthread_t		thread_id;
	t_status		status;
	pthread_mutex_t	status_lock;
	pthread_cond_t	*run;
	pthread_mutex_t	*run_lock;
	int				*run_signal;
	long			start;
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
