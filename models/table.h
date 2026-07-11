/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 18:07:17 by phenry            #+#    #+#             */
/*   Updated: 2026/07/11 16:20:27 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TABLE_H
# define TABLE_H

typedef struct s_args		t_args;
typedef struct s_team		t_team;
typedef struct s_monitor	t_monitor;

typedef struct s_table
{
	t_args		*args;
	t_team		*team;
	t_monitor	*monitor;
}	t_table;

#endif
