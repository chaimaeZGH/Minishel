/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing_ten.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/08/05 19:02:25 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	copy_split(int *i, t_cmdlist *cmd, t_token *args)
{
	int		n;
	char	**spl;

	n = 0;
	spl = ft_split(args->content, ' ');
	while (spl[n] != NULL)
	{
		cmd->cmd[*i] = strdup(spl[n]);
		if (!cmd->cmd[*i])
		{
			free_cmd_array(cmd->cmd, *i);
			cmd->cmd = NULL;
			return (false);
		}
		(*i)++;
		n++;
	}
	n = 0;
	while (spl[n] != NULL)
	{
		free(spl[n]);
		n++;
	}
	free(spl);
	return (true);
}

bool	copy_rest(int *i, t_cmdlist *cmd, t_token *args)
{
	cmd->cmd[*i] = strdup(args->content);
	if (!cmd->cmd[*i])
	{
		free_cmd_array(cmd->cmd, *i);
		cmd->cmd = NULL;
		return (false);
	}
	(*i)++;
	return (true);
}

bool	copy_args_to_cmd(t_cmdlist *cmd, t_token *args, int count)
{
	int	i;

	i = 0;
	cmd->cmd = malloc(sizeof(char *) * (count + 1));
	if (!cmd->cmd)
		return (false);
	while (args)
	{
		if (args->content != NULL)
		{
			if (args->expnd == false)
			{
				if (copy_split(&i, cmd, args) == false)
					return (false);
			}
			else
			{
				if (copy_rest(&i, cmd, args) == false)
					return (false);
			}
		}
		args = args->next;
	}
	cmd->cmd[i] = NULL;
	return (true);
}

void	fill_cmd_from_args(t_cmdlist *cmd)
{
	int	count;

	if (!cmd || !cmd->args)
	{
		cmd->cmd = NULL;
		return ;
	}
	count = count_args(cmd->args);
	if (!copy_args_to_cmd(cmd, cmd->args, count))
		cmd->cmd = NULL;
}

void	fill_double_point(t_tree *tree)
{
	if (!tree)
		return ;
	if (tree->type == comnd)
		fill_cmd_from_args(tree->cmd);
	fill_double_point(tree->left);
	fill_double_point(tree->right);
}
