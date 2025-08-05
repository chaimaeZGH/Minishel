/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rroundi <rroundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:50:48 by rroundi           #+#    #+#             */
/*   Updated: 2025/08/05 22:45:36 by rroundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	out_redir(t_token *out, int *s_s)
{
	int			fd;
	t_token		*curr;

	curr = out;
	*s_s = dup(1);
	if (*s_s == -1)
		return (-1);
	while (curr)
	{
		if (curr->expnd == false)
			return (close(*s_s), ft_putstr_fd("ambiguous redirect\n", 2), -1);
		if (curr->type == OUTredirection)
			fd = open (curr->content, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else if (curr->type == OUTappend)
			fd = open (curr->content, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd == -1)
			return (perror("open out"), -1);
		if (curr->next == NULL)
			dup2(fd, 1);
		close (fd);
		curr = curr->next;
	}
	return (0);
}

int	out_redir_call(t_tree	*tree, int	*s_stdout, t_env	**env)
{
	int	ret;

	if (tree->cmd->out && (tree->cmd->out->type == OUTredirection
			|| tree->cmd->out->type == OUTappend))
	{
		ret = out_redir(tree->cmd->out, s_stdout);
		if (ret == -1)
			return ((*env)->exit_s = 1, ret);
	}
	return (0);
}

int	no_command(int s_stdin, int s_stdout)
{
	if (s_stdin != -1)
	{
		dup2(s_stdin, STDIN_FILENO);
		close(s_stdin);
	}
	if (s_stdout != -1)
	{
		dup2(s_stdout, STDOUT_FILENO);
		close(s_stdout);
	}
	return (0);
}

void	restore(int s_stdin, int s_stdout)
{
	if (s_stdin != -1)
	{
		dup2(s_stdin, STDIN_FILENO);
		close(s_stdin);
	}
	if (s_stdout != -1)
	{
		dup2(s_stdout, STDOUT_FILENO);
		close(s_stdout);
	}
}

int	execute_with_redirections(t_tree *tree, t_env **env)
{
	int	s_stdin;
	int	s_stdout;
	int	ret;

	ret = 0;
	s_stdin = -1;
	s_stdout = -1;
	ret = in_redir_call(tree, &s_stdin, env);
	if (ret == -1)
		return (1);
	ret = out_redir_call(tree, &s_stdout, env);
	if (ret == -1)
		return (1);
	if (!tree->cmd->cmd)
		return (no_command(s_stdin, s_stdout));
	ret = is_builtin(tree);
	if (ret != -1)
		ret = execute_builtins(tree, env, ret);
	else
		ret = exec_bin(tree->cmd->cmd, *env);
	(*env)->exit_s = ret;
	restore(s_stdin, s_stdout);
	return (ret);
}
