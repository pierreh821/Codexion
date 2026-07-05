/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 21:39:54 by phenry            #+#    #+#             */
/*   Updated: 2026/07/05 22:04:17 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

typedef struct s_args	t_args;
typedef struct s_coder	t_coder;

t_args	*clean_args(int argc, char *argv[]);
void	args_validator(t_args *args);

t_coder	**create_coders(int nb);
void	free_coders(t_coder **coders, int nb);

void	error(char *message);

#endif
