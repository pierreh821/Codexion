/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 21:39:54 by phenry            #+#    #+#             */
/*   Updated: 2026/07/15 15:17:10 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <unistd.h>

# include "args.h"
# include "coders.h"
# include "dongle.h"
# include "monitor.h"
# include "table.h"
# include "logger.h"
# include "status.h"

typedef enum e_task			t_task;
typedef enum e_stop_reason	t_stop_reason;
typedef struct s_args		t_args;
typedef struct s_coder		t_coder;
typedef struct s_team		t_team;
typedef struct s_dongle		t_dongle;
typedef struct s_monitor	t_monitor;
typedef struct s_table		t_table;
typedef struct s_log		t_log;
typedef struct s_logger		t_logger;
typedef struct s_status		t_status;


void		describe_tm(t_team *team); //dev

t_table		*init_table(int argc, char *argv[], void *(*work)(void *));
void		join_table(t_table *table);
void		free_table(t_table *table);
int			request_stop(t_table *table, t_stop_reason reason, int	coder_id);

t_args		*clean_args(int argc, char *argv[]);

t_team		*create_team(t_table *table, void *(*work)(void *));
void		wait_team(t_team *team);
void		free_team(t_team *team);
void		team_pause(t_team *team);
void		team_start(t_table *table);
void		set_task(t_coder *coder, t_task task, int update_start);

void		launch_threads(t_table *table, void *(*work)(void *));
int			assign_dongles(t_team *team);

t_dongle	*create_dongle(int id);
void		free_dongles(t_dongle **dongle_set, int nb);

void		create_monitor(t_table *table);
void		free_monitor(t_monitor *monitor);
void		wait_monitor(t_monitor *monitor);
void		check_burnout(t_table *table, int id);
void		*routine(void *arg);
long		time_elapsed(t_monitor *monitor);

void		*work(void *inp);
void		wait_for_start(t_coder *coder);
void		dongle_order(t_coder *coder);
void		take_dongles(t_coder *coder);

void		logger_write(t_coder *coder, char *text);
t_logger	*init_logger(void);
void		free_logger(t_logger *logger);
t_log		*logger_pop(t_logger *logger);
void		*log_export(void *arg);

void		error(t_table *table, char *message);
long		get_time_ms(void);
void		*ft_calloc(size_t nb, size_t sz);

#endif
