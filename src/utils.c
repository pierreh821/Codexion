/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phenry <phenry@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 01:47:25 by phenry            #+#    #+#             */
/*   Updated: 2026/07/16 19:19:31 by phenry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	*ft_calloc(size_t nb, size_t size)
{
	size_t	tot_sz;
	void	*dest;

	if (nb != 0 && size > (size_t)-1 / nb)
		return (NULL);
	tot_sz = nb * size;
	dest = malloc(tot_sz);
	if (dest == NULL)
		return (NULL);
	memset(dest, 0, tot_sz);
	return (dest);
}

char	*ft_strdup(const char *src)
{
	char	*dest;
	int		i;

	dest = ft_calloc(strlen(src) + 1, sizeof(char));
	if (dest == NULL)
		return (NULL);
	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	error(t_table *table, char *message)
{
	static int	in_error = 0;

	if (in_error)
		return ;
	in_error = 1;
	if (message)
		fprintf(stderr, "Error: %s\n", message);
	if (table)
		free_table(table);
	exit(EXIT_FAILURE);
}
