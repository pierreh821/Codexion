/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 04:08:09 by phenry            #+#    #+#             */
/*   Updated: 2026/07/06 18:15:28 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODERS_H
# define CODERS_H
# include <pthread.h>

typedef struct s_coder
{
	int				id;
	pthread_t		thread_id;
	pthread_mutex_t	global_lock;

}	t_coder;

typedef struct s_coders
{
	t_coder			**coders_list;
	pthread_mutex_t	global_lock;
	int				nb;

}	t_coders;

#endif
