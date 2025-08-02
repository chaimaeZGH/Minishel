/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rroundi <rroundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:07:43 by rroundi           #+#    #+#             */
/*   Updated: 2025/07/26 23:13:24 by rroundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int valid_number(char *str)
{
    int i;

    i = 0;
    if (!str || !str[i])
        return (0);
    if (str[i] == '-' || str[i] == '+')
        i++;
    if (!str[i])
        return (0);
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (0);
        i++;
    }
    return (1);
}

int ft_exit(char **av)
{
    int exit_n;

    exit_n = 0;
    if (av[1])
    {
        if(!valid_number(av[1])) // you should add the validating function
        {
            printf("exit\nbash: exit: %s: numeric argument required\n", av[1]);
            exit(255);
        }
        ft_atoi(av[1]);
    }
    if (av[1] && av[2])
    {
        ft_putendl_fd("too many arguments", 2);
        return (1);
    }
    if (av[1])
    {
        exit_n = ft_atoi(av[1]);
        exit_n = exit_n % 256;
    }
    exit(exit_n);
}

// int main ()
// {
//     char *av[3] = {"exit", "2"}; 
//     ft_exit(av);
// }