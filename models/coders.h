/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 04:08:09 by phenry            #+#    #+#             */
/*   Updated: 2026/07/06 17:16:22 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODERS_H
# define CODERS_H
# include <pthread.h>
# include "coder.h"

typedef struct s_coders
{
	t_coder			**coders_list;
	pthread_mutex_t	lock;
	int				nb;

}	t_coders;

#endif
