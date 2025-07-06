/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 22:06:10 by czghoumi          #+#    #+#             */
/*   Updated: 2024/11/16 21:53:06 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	cont1(char const *s1, char const *set)
{
	size_t	i;
	char	*k;

	i = 0;
	while (s1[i])
	{
		k = ft_strchr(set, s1[i]);
		if (k != NULL)
			i++;
		else 
			return (i);
	}
	return (i);
}

static size_t	cont2(char const *s1, char const *set)
{
	size_t	i;
	char	*k;
	size_t	len;

	i = cont1(s1, set);
	len = ft_strlen(s1);
	while (len >= i)
	{
		k = ft_strchr(set, s1[len]);
		if (k != NULL)
			len--;
		else 
			return (len);
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;
	size_t	k;
	char	*ptr;

	if (s1 == NULL || set == NULL)
		return (NULL);
	i = 0;
	j = cont1(s1, set);
	k = cont2(s1, set);
	if (j > k)
		return (ft_strdup(""));
	if (s1[0] == '\0' || set[0] == '\0')
		return (ft_strdup(s1));
	ptr = malloc((k - j) + 2);
	if (ptr == NULL)
		return (NULL);
	ft_strlcpy(ptr, &s1[j], (k - j) + 2);
	return (ptr);
}
