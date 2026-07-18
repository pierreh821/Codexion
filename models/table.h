/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 18:07:17 by phenry            #+#    #+#             */
/*   Updated: 2026/07/18 03:12:50 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TABLE_H
# define TABLE_H

typedef struct s_args		t_args;
typedef struct s_team		t_team;
typedef struct s_monitor	t_monitor;
typedef struct s_status		t_status;
typedef enum e_stop_reason	t_stop_reason;

typedef struct s_table
{
	t_args		*args;
	t_team		*team;
	t_monitor	*monitor;
	t_status	*status;
}	t_table;

t_table	*init_table(int argc, char *argv[], void *(*work)(void *));
void	free_table(t_table *table);
void	join_table(t_table *table);
int		request_stop(t_table *table, t_stop_reason reason, int coder_id);

#endif
