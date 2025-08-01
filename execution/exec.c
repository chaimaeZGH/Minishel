/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rroundi <rroundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 21:42:20 by rroundi           #+#    #+#             */
/*   Updated: 2025/08/01 19:51:27 by rroundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	prepare_heredoc(t_tree_list *tree, char **env)
{
	t_tokenlist	*curr;

	if (tree->type == comnd && tree->cmd && tree->cmd->in)
	{
		curr = tree->cmd->in;
		while (curr)
		{
			if (curr->type == HEREdocument)
			{
				if (heredoc_redir(curr, env) == -1)
					return (-1);
			}
			curr = curr->next;
		}
	}
	if (tree->type == PIPE)
	{
		if (prepare_heredoc(tree->left, env) == -1)
			return (-1);
		if (prepare_heredoc(tree->right, env) == -1)
			return (-1);
	}
	return (0);
}

int	execute(t_tree_list *tree, t_env **env)
{
	int ret = 0;

	if (tree->type == comnd)
	{
		if (tree->cmd)
		{
			if ((tree->cmd->out && (tree->cmd->out->type == OUTredirection || tree->cmd->out->type == OUTappend)) || 
					(tree->cmd->in && (tree->cmd->in->type == INredirection || tree->cmd->in->type == HEREdocument)))
							ret = execute_with_redirections(tree, env);
			else
			{
				if (tree->cmd->cmd)
				{
					ret = is_builtin(tree);
      				if (ret != -1)
        				ret = execute_builtins(tree, env, ret);
        			else
            			ret = exec_bin(tree->cmd->cmd, *env);
      			}
			}
    	}
  }
  else if (tree->type == PIPE)
    ret = execute_pipe(tree, env);
  return (ret);
}

