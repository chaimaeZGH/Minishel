/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rroundi <rroundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/08/05 22:26:38 by rroundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	popul_list(t_token *head, void *content)
{
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
}

t_token	*ft_lstnewn(void *content)
{
	t_token	*head;

	head = malloc(sizeof(t_token));
	if (head == NULL)
		return (NULL);
	head->content = content;
	head->next = NULL;
	head->prev = NULL;
	head->expnd = true;
	if (content == NULL)
	{
		head->type = comnd;
		return (head);
	}
	popul_list(head, content);
	return (head);
}

t_token	*ft_lstlastn(t_token *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
	{
		lst = lst->next;
	}
	return (lst);
}

void	ft_lstadd_backn(t_token **lst, t_token *new)
{
	t_token	*l;

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

void	process_expend_content(t_token *token, char **env, int exit_s)
{
	char	*new_content;

	if (!token || !token->content)
		return ;
	new_content = expand_content(token->content, env, exit_s);
	if (ft_strlen(new_content) == 0 && 
		(token->type == INredirection || token->type == OUTredirection))
		token->expnd = false;
	else
		token->expnd = true;
	if (ft_strchr(token->content, '$') != NULL && 
		ft_count(new_content, ' ') > 1)
		token->expnd = false;
	free(token->content);
	token->content = new_content;
}
