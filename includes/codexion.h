/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 21:39:54 by phenry            #+#    #+#             */
/*   Updated: 2026/07/13 01:27:17 by phenry           ###   ########.fr       */
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

typedef enum e_state		t_state;
typedef struct s_args		t_args;
typedef struct s_coder		t_coder;
typedef struct s_team		t_team;
typedef struct s_dongle		t_dongle;
typedef struct s_monitor	t_monitor;
typedef struct s_table		t_table;
typedef struct s_logger		t_logger;

void		error(char *message); // dev
void		*foo(void *arg);
void		describe_tm(t_team *team);

t_table		*init_table(int argc, char *argv[], void *(*work)(void *));
void		free_table(t_table *table);

t_args		*clean_args(int argc, char *argv[]);
void		args_validator(t_args *args);

t_team		*create_team(t_table *table, void *(*work)(void *));
void		wait_coders(t_team *coders);
void		free_team(t_team *coders);
void		team_pause(t_team *team);
void		team_start(t_table *table);

void		launch_threads(t_table *table, void *(*work)(void *));
void		assign_dongles(t_team *team);

t_dongle	*create_dongle(int id);
void		free_dongle(t_dongle *dongle);

void		create_monitor(t_table *table);
void		end_wait_monitor(t_monitor *monitor);

void		*work(void *inp);
void		wait_for_start(t_coder *coder);
void		dongle_order(t_coder *coder);
void		take_dongles(t_coder *coder);

void		new_log(t_logger *log, char	*text);
t_logger	*init_logger(void);
void		free_logger(t_logger *logger);

#endif
