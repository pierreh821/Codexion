/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 02:06:50 by pierre            #+#    #+#             */
/*   Updated: 2026/07/07 19:23:25 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DONGLE_H
# define DONGLE_H
# include <pthread.h>

typedef struct s_dongle
{
	pthread_mutex_t	lock;
	int				id;
}	t_dongle;

#endif
