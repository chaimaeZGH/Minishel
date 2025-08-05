/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_tree_second.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/08/05 19:03:18 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fill_tree(t_cmdlist *cmd, t_token *original)
{
	t_token	*head;

	if (original == NULL)
		return ;
	head = original;
	while (head != NULL)
	{
		if (head->type == comnd)
			ft_lstadd_backn(&cmd->args, 
				ft_lstnewnn(ft_strdup(head->content), head->type));
		if (head->type == INredirection || head->type == HEREdocument)
			ft_lstadd_backn(&cmd->in, 
				ft_lstnewnn(ft_strdup(head->content), head->type));
		if (head->type == OUTappend || head->type == OUTredirection)
			ft_lstadd_backn(&cmd->out, 
				ft_lstnewnn(ft_strdup(head->content), head->type));
		head = head->next;
	}
}

t_token	*ft_lstnewnn(void *content, t_type_list ttype)
{
	t_token	*head;

	head = malloc(sizeof(t_token));
	if (head == NULL)
		return (NULL);
	head->content = content;
	head->next = NULL;
	head->prev = NULL;
	head->type = ttype;
	head->expnd = true;
	return (head);
}

t_tree	*create_tree_node(void)
{
	t_tree	*tree;

	tree = malloc(sizeof(t_tree));
	if (!tree)
		return (NULL);
	tree->right = NULL;
	tree->left = NULL;
	tree->cmd = NULL;
	return (tree);
}

t_cmdlist	*create_cmd_node(t_tree *tree)
{
	tree->cmd = malloc(sizeof(t_cmdlist));
	if (!tree->cmd)
	{
		free(tree);
		return (NULL);
	}
	tree->cmd->here_doc_fd = -1;
	tree->cmd->cmd = NULL;
	tree->cmd->args = NULL;
	tree->cmd->in = NULL;
	tree->cmd->out = NULL;
	return (tree->cmd);
}

t_tree	*create_tree(t_token **head)
{
	t_tree	*tree;
	t_token	*last_p;

	if (head == NULL || *head == NULL)
		return (NULL);
	tree = create_tree_node();
	if (!tree)
		return (NULL);
	last_p = last_pipe(*head);
	if (last_p == NULL)
	{
		tree->cmd = create_cmd_node(tree);
		if (!tree->cmd)
			return (NULL);
		tree->type = comnd;
		fill_tree(tree->cmd, *head);
		free_list(*head);
	}
	else
		remove_nodes(last_p, tree, head);
	return (tree);
}
