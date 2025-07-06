/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 01:55:12 by czghoumi          #+#    #+#             */
/*   Updated: 2024/11/17 15:00:12 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	needlelen;
	size_t	i;

	if (!haystack && len == 0)
		return (NULL);
	if (needle[0] == '\0')
		return ((char *)haystack);
	else if (haystack[0] == '\0')
		return (NULL);
	needlelen = ft_strlen((char *)needle);
	i = 0;
	if (len >= needlelen)
	{
		while (i <= len - needlelen && haystack[i])
		{
			if (ft_strncmp(&haystack[i], needle, needlelen) == 0)
				return ((char *)haystack + i);
			i++;
		}
	}
	return (NULL);
}
