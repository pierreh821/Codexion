/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 21:39:54 by phenry            #+#    #+#             */
/*   Updated: 2026/07/06 18:52:49 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

typedef struct s_args		t_args;
typedef struct s_coder		t_coder;
typedef struct s_team		t_team;
typedef struct s_dongle		t_dongle;
typedef struct s_dongle_set	t_dongle_set;
typedef struct s_monitor	t_monitor;

t_args			*clean_args(int argc, char *argv[]);
void			args_validator(t_args *args);

t_team			*create_coders(int nb);
void			wait_coders(t_team *coders);
void			free_coders(t_team *coders);

t_dongle_set	*create_dongles(int nb);
void			free_dongles(t_dongle_set *dongles);

void			error(char *message);

#endif
