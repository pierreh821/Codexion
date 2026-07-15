/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 11:16:45 by phenry            #+#    #+#             */
/*   Updated: 2026/07/15 11:29:42 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STOP_H
# define STOP_H

# include <pthread.h>

typedef enum	e_stop_reason
{
	STOP_NONE,
	STOP_COMPLETED,
	STOP_BURNOUT,
	STOP_FATAL,
}	t_stop_reason;

typedef struct s_status
{
	pthread_mutex_t	lock;
	t_stop_reason	reason;
	int				coder_id;
}	t_status;

#endif
