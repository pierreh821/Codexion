/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 20:42:48 by phenry            #+#    #+#             */
/*   Updated: 2026/07/04 21:54:39 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODER_H
# define CODER_H

typedef struct s_coder
{
	int			id;
	pthread_t	thread;

}	t_coder;

#endif
