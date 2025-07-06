/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 01:19:41 by czghoumi          #+#    #+#             */
/*   Updated: 2024/11/16 21:50:35 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count(char const *s, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] != '\0')
			j++;
		while (s[i] != c && s[i] != '\0')
			i++;
	}
	return (j);
}

static char	*ft_strdp(const char *s1, int i, char c)
{
	char	*ptr;
	int		len;
	int		j;

	len = 0;
	j = 0;
	while (s1[i + len] != '\0' && s1[i + len] != c)
		len++;
	ptr = malloc(len + 1);
	if (ptr == NULL)
		return (NULL);
	while (j < len)
	{
		ptr[j] = s1[i + j];
		j++;
	}
	ptr[j] = '\0';
	return (ptr);
}

static char	**rempli(char **ptr, char const *s, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] != '\0') 
		{
			ptr[j] = ft_strdp(s, i, c);
			if (ptr[j] == NULL) 
			{
				while (j > 0) 
					free(ptr[--j]);
				return (free(ptr), (NULL));
			}
			j++;
		}
		while (s[i] != c && s[i] != '\0')
			i++;
	}
	ptr[j] = NULL;
	return (ptr);
}

char	**ft_split(char const *s, char c)
{
	char	**ptr;
	int		count;

	if (s == NULL)
		return (NULL);
	count = ft_count(s, c);
	ptr = malloc ((count + 1) * sizeof(char *));
	if (ptr == NULL)
		return (NULL);
	return (rempli(ptr, s, c));
}
