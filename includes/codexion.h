/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 21:39:54 by phenry            #+#    #+#             */
/*   Updated: 2026/07/14 14:42:42 by phenry           ###   ########.fr       */
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

typedef enum e_status		t_status;
typedef struct s_args		t_args;
typedef struct s_coder		t_coder;
typedef struct s_team		t_team;
typedef struct s_dongle		t_dongle;
typedef struct s_monitor	t_monitor;
typedef struct s_table		t_table;
typedef struct s_log		t_log;
typedef struct s_logger		t_logger;

void		error(char *message); // dev
void		*foo(void *arg);
void		describe_tm(t_team *team);

t_table		*init_table(int argc, char *argv[], void *(*work)(void *));
void		free_table(t_table *table);
t_table		*get_table(t_table *set_table);

t_args		*clean_args(int argc, char *argv[]);
void		args_validator(t_args *args);

t_team		*create_team(t_table *table, void *(*work)(void *));
void		wait_coders(t_team *coders);
void		free_team(t_team *coders);
void		team_pause(t_team *team);
void		team_start(t_table *table);
void		set_status(t_coder *coder, t_status status, int update_start);

void		launch_threads(t_table *table, void *(*work)(void *));
void		assign_dongles(t_team *team);

t_dongle	*create_dongle(int id);
void		free_dongles(t_dongle **dongle_set, int nb);

void		create_monitor(t_table *table);
void		end_wait_monitor(t_monitor *monitor);
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

long		get_time_ms(void);

#endif
