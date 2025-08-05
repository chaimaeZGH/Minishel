/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rroundi <rroundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 21:42:20 by rroundi           #+#    #+#             */
/*   Updated: 2025/08/05 20:45:47 by rroundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	prepare_heredoc(t_tree_list *tree, char **env, int exit_s)
{
	t_tokenlist	*curr;

	if (tree->type == comnd && tree->cmd && tree->cmd->in)
	{
		curr = tree->cmd->in;
		while (curr)
		{
			if (curr->type == HEREdocument)
			{
				if (heredoc_redir(curr, env, exit_s) == -1)
					return (-1);
			}
			curr = curr->next;
		}
	}
	if (tree->type == PIPE)
	{
		if (prepare_heredoc(tree->left, env, exit_s) == -1)
			return (-1);
		if (prepare_heredoc(tree->right, env, exit_s) == -1)
			return (-1);
	}
	return (0);
}

int	execute_helper(t_tree_list *tree, t_env **env, int ret)
{
	if (tree->cmd->cmd)
	{
		ret = is_builtin(tree);
		if (ret != -1)
			ret = execute_builtins(tree, env, ret);
		else
			ret = exec_bin(tree->cmd->cmd, *env);
	}
	return (ret);
}

int	execute(t_tree_list *tree, t_env **env)
{
	int	ret;

	ret = 0;
	if (tree)
	{
	if (tree->type == comnd)
	{
		if (tree->cmd)
		{
			if ((tree->cmd->out && 
					(tree->cmd->out->type == OUTredirection 
						|| tree->cmd->out->type == OUTappend)) 
				|| (tree->cmd->in && (tree->cmd->in->type == INredirection 
						|| tree->cmd->in->type == HEREdocument)))
				ret = execute_with_redirections(tree, env);
			else
				ret = execute_helper(tree, env, ret);
		}
	}
	else if (tree->type == PIPE)
		ret = execute_pipe(tree, env);
	}
	return (ret);
}
