/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 17:47:20 by phenry            #+#    #+#             */
/*   Updated: 2026/07/06 17:50:10 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DONGLE_H
# define DONGLE_H
# include <pthread.h>

typedef struct s_dongle
{
	pthread_mutex_t	lock;
}	t_dongle;

#endif
