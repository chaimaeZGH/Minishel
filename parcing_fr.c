/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing_fr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/08/05 17:47:34 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_node(t_token *node)
{
	free(node->content);
	free(node);
}

void	add_new_conten(t_token *current, t_token *prevv, char *new_c)
{
	size_t	new_len;

	new_len = ft_strlen(current->content) + ft_strlen(prevv->content) + 1;
	new_c = malloc(new_len);
	if (!new_c)
		return ;
	ft_strlcpy(new_c, prevv->content, new_len);
	ft_strlcat(new_c, current->content, new_len);
	free(prevv->content);
	prevv->content = new_c;
	prevv->next = current->next;
	if (current->next)
		current->next->prev = prevv;
	free_node(current);
}

char	*map_type(t_type_list type)
{
	if (type == 0)
		return ("command");
	if (type == 1)
		return ("pipe");
	if (type == 2)
		return ("redirection in");
	if (type == 3)
		return ("redirection out");
	if (type == 4)
		return ("redirection heredoc");
	if (type == 5)
		return ("redirection append");
	return (NULL);
}

void	print_nodes(t_token *head)
{
	t_token	*current;

	current = head;
	while (current != NULL)
	{
		printf("Content: [%s], " "type: %s \n", 
			current->content, map_type(current->type));
		current = current->next;
	}
}

void	free_list(t_token *head)
{
	t_token	*tmp;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp->content);
		free(tmp);
	}
}
