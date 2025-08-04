/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rroundi <rroundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 22:46:41 by rroundi           #+#    #+#             */
/*   Updated: 2025/08/04 22:51:34 by rroundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	print_error(char *cmd, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
	return (1);
}

int	error_msg(char *cmd, char *msg, char *av)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(av, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
	return (1);
}

int	sublen(char *av)
{
	int	i;

	i = 0;
	while (av[i] && av[i] != '=')
		i++;
	return (i);
}

int	handle_exec(char **cmd)
{
	if (errno == ENOENT)
	{
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(" :command not found", 2);
		return (127);
	}
	else if (errno == EACCES)
	{
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(" :permission denied", 2);
		return (126);
	}
	else
	{
		ft_putstr_fd("execve failed", 2);
		return (1);
	}
}

void	no_fd(char	*file_name)
{
	perror("failed to open heredoc");
	free(file_name);
	exit(1);
}
