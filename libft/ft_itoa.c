/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 01:49:39 by czghoumi          #+#    #+#             */
/*   Updated: 2024/11/16 21:49:20 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_len(long c)
{
	int	i;

	i = 0;
	if (c == 0)
		return (1);
	if (c < 0)
	{
		c = -c;
		i++;
	}
	while (c > 0)
	{
		c = c / 10;
		i++;
	}
	return (i);
}

static char	*shihaja(int c, char *ptr, long nb)
{
	if (nb < 0)
	{
		nb = -nb;
		ptr[0] = '-';
	}
	if (nb == 0)
	{
		ptr[0] = '0';
		return (ptr);
	}
	while (nb > 0)
	{
		ptr[c] = nb % 10 + '0';
		nb = nb / 10;
		c--;
	}
	return (ptr);
}

char	*ft_itoa(int n)
{
	char	*ptr;
	long	nb;
	int		c;

	nb = n;
	c = ft_len(nb);
	ptr = malloc(c + 1);
	if (ptr == NULL)
		return (NULL);
	ptr[c] = '\0';
	return (shihaja(c - 1, ptr, nb));
}
