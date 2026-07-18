/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 04:08:09 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 15:09:00 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODERS_H
# define CODERS_H
# include <pthread.h>
# include <sys/time.h>
# include "dongle.h"
# include "table.h"

typedef enum e_task
{
	COMPILING,
	DEBUGGING,
	REFACTORING,
	SUSPEND
}	t_task;

typedef struct s_coder
{
	int				id;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	t_dongle		*first;
	t_dongle		*second;
	pthread_t		thread_id;
	t_task			task;
	pthread_mutex_t	task_lock;
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

void	compile(t_coder *coder);
void	debug(t_coder *coder);
void	refactor(t_coder *coder);
void	*work(void *inp);

void	dongle_order(t_coder *coder);
int		take_dongles(t_coder *coder);
int		work_cycle(t_coder *coder);

int		alloc_team(t_table *table);
int		assign_cond(t_table *table);
int		assign_coders(t_table *table);
int		launch_threads(t_table *table, void *(*work)(void *));
int		create_team(t_table *table, void *(*work)(void *));

void	wait_for_start(t_coder *coder);
void	wait_team(t_team *team);
void	team_start(t_table *table);
void	team_pause(t_team *team);
void	set_task(t_coder *coder, t_task task, int update_start);

void	free_team(t_team *team);

#endif
