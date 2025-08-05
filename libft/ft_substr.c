/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 17:27:21 by czghoumi          #+#    #+#             */
/*   Updated: 2024/11/16 21:53:20 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count(char const *s, size_t len)
{
	size_t	i;

	i = 0;
	while (s[i] && i < len)
		i++;
	return (i);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ptr;
	size_t	b;
	size_t	i;

	if (!s)
		return (NULL);
	if ((size_t)start >= ft_strlen (s))
		return (ft_strdup(""));
	b = count(&s[start], len);
	ptr = malloc(b + 1);
	if (ptr == NULL)
		return (NULL);
	i = 0;
	while (i < b)
	{
		ptr[i] = s[start + i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}
