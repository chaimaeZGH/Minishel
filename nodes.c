/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/07/06 21:13:06 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tokenlist *ft_lstnewn(void *content)
{
	t_tokenlist *head;

	head = malloc(sizeof(t_tokenlist));
	if (head == NULL)
		return NULL;
		
	head->content = content;
	head->next = NULL;
	head->prev = NULL;
	
	if (content == NULL) 
	{
		head->type = comnd;
		return head;
	}
	
	if (ft_strncmp("|", (char *)content, 1) == 0)
		head->type = PIPE;
	else if (ft_strncmp("<<", (char *)content, 2) == 0)
		head->type = HEREdocument;
	else if (ft_strncmp(">>", (char *)content, 2) == 0)
		head->type = OUTappend;
	else if (ft_strncmp("<", (char *)content, 1) == 0)
		head->type = INredirection;
	else if (ft_strncmp(">", (char *)content, 1) == 0)
		head->type = OUTredirection;
	else if (ft_strncmp("\"", (char *)content, 1) == 0)
		head->type = comnd;
	else if (ft_strncmp("\'", (char *)content, 1) == 0)
		head->type = comnd;
	else
		head->type = comnd;
		
	return head;
}

t_tokenlist	*ft_lstlastn(t_tokenlist *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
	{
		lst = lst->next;
	}
	return (lst);
}

void	ft_lstadd_backn(t_tokenlist **lst, t_tokenlist *new)
{
	t_tokenlist	*l;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	l = ft_lstlastn(*lst);
	l->next = new;
	new->prev = l;
}