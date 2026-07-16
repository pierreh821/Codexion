/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 02:06:50 by pierre            #+#    #+#             */
/*   Updated: 2026/07/16 19:17:27 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DONGLE_H
# define DONGLE_H
# include <pthread.h>

typedef struct s_dongle
{
	int				id;
	t_coder			**waitlist;
	int				waitlist_sz;
	pthread_mutex_t	waitlist_lock;
}	t_dongle;

#endif
