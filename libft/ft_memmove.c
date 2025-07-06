/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 22:54:52 by czghoumi          #+#    #+#             */
/*   Updated: 2024/11/18 22:01:57 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char	*a;
	char	*b;

	a = (char *)dst;
	b = (char *)src;
	if (dst == src)
		return (dst);
	if (a > b)
	{
		while (len > 0)
		{
			len--;
			a[len] = b[len]; 
		}
		return (dst);
	}
	return (ft_memcpy(dst, src, len));
}
