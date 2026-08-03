/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 02:51:54 by phenry            #+#    #+#             */
/*   Updated: 2026/08/03 00:55:19 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stddef.h>

void	*ft_calloc(size_t nb, size_t size);
char	*ft_strdup(const char *src);
long	get_time_ms(void);
long	get_time_us(void);
int		ft_isnumber(char *s);

#endif
