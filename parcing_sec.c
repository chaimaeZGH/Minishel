/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing_sec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/08/04 23:50:01 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_error_msg(int errnum)
{
	if (errnum == 0)
		printf("Syntax error: unexpected token ; or \\\n");
	if (errnum == 1)
		printf("syntax error near unexpected token\n");
	if (errnum == 2)
		printf("syntax error near '|'\n");
}

int	syntax_erreur(t_token *head)
{
	int			i;
	int			j;
	t_token		*current;

	i = 0;
	j = 0;
	current = head;
	while (current != NULL)
	{
		if (current->type != comnd && current->next == NULL)
			return (ft_error_msg(1), ++j);
		if (current->type == PIPE && 
			(current->prev == NULL || current->prev->type != comnd))
			return (ft_error_msg(2), ++j);
		if ((current->type != PIPE && current->type != comnd) && 
			(current->next->type != comnd && current->next->type != PIPE))
			return (ft_error_msg(1), ++j);
		current = current->next;
	}
	return (j);
}

int	chaeck_content(t_token *node)
{
	if ((node->type == INredirection && 
			ft_strncmp(node->content, "<", 1) == 0) || 
		(node->type == OUTredirection && 
			ft_strncmp(node->content, ">", 1) == 0) || 
		(node->type == HEREdocument && 
			ft_strncmp(node->content, "<<", 2) == 0) || 
		(node->type == OUTappend && 
			ft_strncmp(node->content, ">>", 2) == 0))
		return (1);
	else
		return (0);
}

void	continue_merge(t_token **head)
{
	t_token	*current;
	t_token	*next_node;

	current = *head;
	while (current)
	{
		if (chaeck_content(current) == 1)
		{
			next_node = current->next;
			if (next_node)
			{
				next_node->type = current->type;
				if (current->prev)
					current->prev->next = next_node;
				else
					*head = next_node;
				next_node->prev = current->prev;
				free_node(current);
				current = next_node;
				continue ;
			}
		}
		current = current->next;
	}
}

void	merge_file_cmd(t_token **head)
{
	if (!head || !*head)
		return ;
	continue_merge(head);
}
