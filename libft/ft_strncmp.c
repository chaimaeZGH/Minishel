/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 01:15:22 by czghoumi          #+#    #+#             */
/*   Updated: 2024/11/15 23:24:44 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;
	int		def;

	i = 0;
	def = 0;
	while (((unsigned char)s1[i] || (unsigned char)s2[i]) && i < n)
	{
		def = (unsigned char)s1[i] - (unsigned char)s2[i];
		if (def == 0)
			i++;
		else
			return (def);
	}
	return (0);
}
