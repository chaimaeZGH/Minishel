/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rroundi <rroundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 16:31:33 by czghoumi          #+#    #+#             */
/*   Updated: 2025/07/26 23:08:09 by rroundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// int	ft_atoi(const char *str)
// {
// 	unsigned long long	result;
// 	int					sign;
// 	int					i;

// 	i = 0;
// 	sign = 1;
// 	result = 0;
// 	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
// 		i++;
// 	if (str[i] == '-' || str[i] == '+') 
// 	{
// 		if (str[i] == '-')
// 			sign = -1;
// 		i++;
// 	}
// 	while (str[i] >= '0' && str[i] <= '9') 
// 	{
// 		result = (result * 10) + (str[i] - '0');
// 		if (result > 9223372036854775807 && sign == -1)
// 			return (0);
// 		if (result >= 9223372036854775807 && sign == 1)
// 			return (-1);
// 		i++;
// 	}
// 	return (result * sign);
// }
long long    ft_atoi(char *str)
{
    int            i;
    long long    res;
    int            sign;

    i = 0;
    sign = 1;
    res = 0;
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
    while (str[i] >= '0' && str[i] <= '9')
    {
        if (res > (LLONG_MAX - (str[i] - '0')) / 10)
        {
            printf("exit\nbash: exit: %s: numeric argument required\n", str);
            exit(255);
        }
        res = res * 10 + (str[i] - '0');
        i++;
    }
    return (res * sign);
}