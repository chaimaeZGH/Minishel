/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rroundi <rroundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 22:12:39 by rroundi           #+#    #+#             */
/*   Updated: 2025/08/05 22:45:48 by rroundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	for_inredir(t_token *curr, t_token *in)
{
	int	fd;

	fd = open (in->content, O_RDONLY);
	if (fd == -1)
	{
		perror("open in");
		return (-1);
	}
	if (curr->next == NULL)
		dup2(fd, 0);
	close(fd);
	return (0);
}

int	for_heredoc(t_token *curr)
{
	int		fd;
	char	*file_name;

	file_name = generate_filename(curr->fd);
	fd = open (file_name, O_RDONLY);
	unlink(file_name);
	if (fd == -1)
	{
		perror("heredoc");
		return (-1);
	}
	if (curr->next == NULL)
	{
		dup2(fd, 0);
		close(fd);
	}
	close (fd);
	free(file_name);
	return (0);
}

int	in_redir(t_token *in, int *s_stdin)
{
	t_token	*curr;

	*s_stdin = dup(0);
	if (*s_stdin == -1)
		return (-1);
	curr = in;
	while (curr)
	{
		if (curr->type == INredirection)
		{
			if (curr->expnd == false)
			{
				ft_putstr_fd("ambiguous redirect\n", 2);
				return (close(*s_stdin), -1);
			}
			if (for_inredir(curr, in) == -1)
				return (-1);
		}
		if (curr->type == HEREdocument)
			if (for_heredoc(curr) == -1)
				return (-1);
		curr = curr->next;
	}
	return (0);
}

int	in_redir_call(t_tree *tree, int *s_stdin, t_env **env)
{
	int	ret;

	if (tree->cmd->in)
	{
		if (tree->cmd->in->type == INredirection
			|| tree->cmd->in->type == HEREdocument)
		{
			ret = in_redir(tree->cmd->in, s_stdin);
			if (ret == -1)
				return ((*env)->exit_s = 1, ret);
		}
	}
	return (0);
}
