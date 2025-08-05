/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing_nin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rroundi <rroundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/08/05 22:22:24 by rroundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_remove_quots(t_tree *root)
{
	t_token	*current_arg;
	t_token	*current_in;
	t_token	*current_out;

	if (root->type == comnd && root->cmd)
	{
		if (root->cmd->args)
		{
			current_arg = root->cmd->args;
			call_remove_quotes(current_arg);
		}
		if (root->cmd->in)
		{
			current_in = root->cmd->in;
			call_remove_quotes(current_in);
		}
		if (root->cmd->out)
		{
			current_out = root->cmd->out;
			call_remove_quotes(current_out);
		}
	}
}

void	remove_quots(t_tree *root)
{
	if (!root)
		return ;
	check_remove_quots(root);
	remove_quots(root->right);
	remove_quots(root->left);
}

int	ft_count(char *s, char c)
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

int	count_args(t_token *args)
{
	int	count;

	count = 0;
	while (args)
	{
		if (args->expnd == false)
		{
			count = count + ft_count(args->content, ' ');
		}
		else
			count++;
		args = args->next;
	}
	return (count);
}

void	free_cmd_array(char **cmd, int i)
{
	while (i > 0)
		free(cmd[--i]);
	free(cmd);
}
