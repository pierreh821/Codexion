/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 21:39:54 by phenry            #+#    #+#             */
/*   Updated: 2026/07/09 15:22:12 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

typedef struct s_args		t_args;
typedef struct s_coder		t_coder;
typedef struct s_team		t_team;
typedef struct s_dongle		t_dongle;
typedef struct s_monitor	t_monitor;

void		error(char *message); // dev
void		*foo(void *arg);
void		describe_tm(t_team *team);

t_args		*clean_args(int argc, char *argv[]);
void		args_validator(t_args *args);

t_team		*create_coders(int nb, t_monitor *monitor);
void		wait_coders(t_team *coders);
void		free_coders(t_team *coders);

t_dongle	*create_dongle(int id);
void		free_dongle(t_dongle *dongle);

t_monitor	*create_monitor(void);
void		end_wait_monitor(t_monitor *monitor);

#endif
