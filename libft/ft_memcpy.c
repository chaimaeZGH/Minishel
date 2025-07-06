/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 16:13:07 by czghoumi          #+#    #+#             */
/*   Updated: 2024/11/15 23:23:46 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char	*st1;
	char	*st2;
	size_t	i;

	st1 = (char *) dst;
	st2 = (char *) src;
	i = 0;
	if (dst == src || n == 0)
		return (dst);
	while (i < n)
	{
		st1[i] = st2[i];
		i++;
	}
	return (dst);
}
