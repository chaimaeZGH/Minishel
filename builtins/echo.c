/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rroundi <rroundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:43:59 by rroundi           #+#    #+#             */
/*   Updated: 2025/08/04 19:45:24 by rroundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	if_flag(char *av)
{
	int	i;

	i = 0;
	if (!av[i] || av[i] != '-')
		return (0);
	i++;
	if (av[i] == '\0')
		return (0);
	while (av[i] && av[i] == 'n')
		i++;
	if (av[i] != '\0')
		return (0);
	return (1);
}

int	ft_echo(char **av)
{
	int	i;
	int	flag;

	flag = 1;
	i = 1;
	while (av[i] && if_flag(av[i]))
	{
		flag = 0;
		i++;
	}
	while (av[i])
	{
		printf("%s", av[i]);
		if (av[i + 1])
			printf(" ");
		i++;
	}
	if (flag == 1)
		printf("\n");
	return (0);
}
