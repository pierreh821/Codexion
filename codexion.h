/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 21:39:54 by phenry            #+#    #+#             */
/*   Updated: 2026/07/06 17:23:48 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

typedef struct s_args	t_args;
typedef struct s_coder	t_coder;
typedef struct s_coders	t_coders;

t_args		*clean_args(int argc, char *argv[]);
void		args_validator(t_args *args);

t_coders	*create_coders(int nb);
void		wait_coders(t_coders *coders);
void		free_coders(t_coders *coders);

void		error(char *message);

#endif
