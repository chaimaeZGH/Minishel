/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_tree_first.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/08/05 19:02:25 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd(char **cmd)
{
	int	i;

	i = 0;
	if (!cmd)
		return ;
	while (cmd[i] != NULL)
	{
		free(cmd[i]);
		i++;
	}
	free(cmd);
}

void	free_s_cmd(t_cmdlist *cmd)
{
	if (!cmd)
		return ;
	free_list(cmd->args);
	free_list(cmd->in);
	free_list(cmd->out);
	free_cmd(cmd->cmd);
	free(cmd);
}

void	free_tree(t_tree *tree)
{
	if (!tree)
		return ;
	if (tree->type != PIPE)
		free_s_cmd(tree->cmd);
	free_tree(tree->right);
	free_tree(tree->left);
	free(tree);
}

t_token	*last_pipe(t_token *head)
{
	t_token	*current;

	if (!head)
		return (NULL);
	current = head;
	while (current->next != NULL)
		current = current->next;
	while (current != NULL)
	{
		if (current->type == PIPE)
			return (current);
		else
			current = current->prev;
	}
	return (current);
}

void	remove_nodes(t_token *last_p, t_tree *tree, t_token **head)
{
	t_token	*previous;
	t_token	*right_part;

	previous = last_p->prev;
	previous->next = NULL;
	right_part = last_p->next;
	right_part->prev = NULL;
	free(last_p->content);
	free(last_p);
	tree->type = PIPE;
	tree->right = create_tree(&right_part);
	tree->left = create_tree(head);
}
