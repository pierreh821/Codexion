/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 18:07:17 by phenry            #+#    #+#             */
/*   Updated: 2026/07/11 15:45:39 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TABLE_H
# define TABLE_H

# include "codexion.h"

typedef struct s_table
{
	t_args		*args;
	t_team		*team;
	t_monitor	*monitor;
}	t_table;

#endif
