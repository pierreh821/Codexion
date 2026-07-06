/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 20:42:48 by phenry            #+#    #+#             */
/*   Updated: 2026/07/06 01:31:42 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODER_H
# define CODER_H
# include <pthread.h>

typedef struct s_coder
{
	int				id;
	pthread_t		thread_id;
	pthread_mutex_t	*lock;

}	t_coder;

#endif
