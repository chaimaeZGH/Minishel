/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:59:53 by rroundi           #+#    #+#             */
/*   Updated: 2025/08/05 20:53:45 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2) 
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

int	is_builtin(t_tree *tree)
{
	if (ft_strcmp(tree->cmd->cmd[0], "echo") == 0)
		return (1);
	else if (ft_strcmp(tree->cmd->cmd[0], "cd") == 0)
		return (2);
	else if (ft_strcmp(tree->cmd->cmd[0], "env") == 0)
		return (3);
	else if (ft_strcmp(tree->cmd->cmd[0], "exit") == 0)
		return (4);
	else if (ft_strcmp(tree->cmd->cmd[0], "export") == 0)
		return (5);
	else if (ft_strcmp(tree->cmd->cmd[0], "pwd") == 0)
		return (6);
	else if (ft_strcmp(tree->cmd->cmd[0], "unset") == 0)
		return (7);
	return (-1);
}

int	execute_builtins(t_tree *tree, t_env **env, int flag)
{
	int	ret;

	ret = -1;
	if (flag == 1)
		ret = ft_echo(tree->cmd->cmd);
	else if (flag == 2)
		ret = ft_cd(env, tree->cmd->cmd);
	else if (flag == 3)
		ret = ft_call_env(*env, tree->cmd->cmd);
	else if (flag == 4)
		ret = ft_exit (tree->cmd->cmd);
	else if (flag == 5)
		ret = ft_export(tree->cmd->cmd, env);
	else if (flag == 6)
		ret = ft_pwd();
	else if (flag == 7)
		ret = ft_unset(env, tree->cmd->cmd);
	(*env)->exit_s = ret;
	return (ret);
}
