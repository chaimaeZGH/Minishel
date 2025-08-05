/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:23:20 by czghoumi          #+#    #+#             */
/*   Updated: 2024/11/18 12:42:43 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	x;
	size_t	y;
	size_t	j;

	x = ft_strlen((char *)src);
	j = 0;
	if (dstsize == 0)
		return (x);
	y = ft_strlen(dst);
	if (((dstsize - 1) - y) == 0)
		return (x + y);
	if (dstsize <= y)
		return (dstsize + x);
	while ((j < ((dstsize - 1) - y)) && src[j] != '\0')
	{
		dst[y + j] = src[j];
		j++;
	}
	dst[y + j] = '\0';
	return (x + y);
}
