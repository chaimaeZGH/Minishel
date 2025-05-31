/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merj.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/05/31 21:53:02 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	x;
	size_t	y;
	size_t	j;

	x = ft_strlen((char *)src);
	j = 0;
	if (dstsize == 0)
		return (x);
	y = ft_strlen(dst);
	if (((dstsize - 1) - y) == 0)
		return (x + y);
	if (dstsize <= y)
		return (dstsize + x);
	while ((j < ((dstsize - 1) - y)) && src[j] != '\0')
	{
		dst[y + j] = src[j];
		j++;
	}
	dst[y + j] = '\0';
	return (x + y);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (dstsize != 0)
	{
		while (src[i] && i < (dstsize - 1))
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (ft_strlen((char *)src));
}

void merge_quotes(t_list *start, t_list *end)
{
	char *new_content;
	t_list *node_to_merge;
	t_list *next_node;

	if (!start || !start->content) return;

	node_to_merge = start->next;
	
	while (node_to_merge != NULL && node_to_merge != end)
	{
		if (node_to_merge->content) 
        {
			size_t new_len = ft_strlen(start->content) + ft_strlen(node_to_merge->content) + 1;
			new_content = malloc(new_len);
			if (!new_content) 
				return;
			ft_strlcpy(new_content, start->content, new_len);
			ft_strlcat(new_content, node_to_merge->content, new_len);
			
			free(start->content);
			start->content = new_content;
		}
		next_node = node_to_merge->next;
		start->next = next_node;
		if (next_node)
			next_node->prev = start;

		free(node_to_merge->content);
		free(node_to_merge);
		node_to_merge = next_node;
	}
}

void what_to_merge(t_list **head)
{
	if (!head || !*head) 
		return;

	t_list *current;
	char quote_char;
	t_list *end_node;
	t_list *last_node;

	current = *head;
	while (current)
	{
		if (current->content && (current->content[0] == '\"' || current->content[0] == '\''))
		{
			quote_char = current->content[0];
			end_node = current->next;
			last_node = NULL;

			while (end_node && (!end_node->content || end_node->content[0] != quote_char)) {
				last_node = end_node;
				end_node = end_node->next;
			}
			if (end_node) 
			{
				merge_quotes(current, end_node->next);
				current = end_node->next;
			} 
			else if (last_node) 
			{
				merge_quotes(current, NULL);
				current = NULL;
			} 
			else 
				current = current->next;
		} 
		else 
			current = current->next;
	}
}


void merge_cmd_quoat(t_list **head)
{
	if (!head || !*head) 
		return;

	char *new_content;
	t_list *current;
	t_list *next_nod;
	int len;
	current = *head;
	
	while (current && current->next)
	{
		len = ft_strlen(current->content);
		next_nod=current->next;
		
		if (current->content && next_nod->content && (current->index == 0 && next_nod->index == 0) && (next_nod->content[0] == '\"' || next_nod->content[0] == '\'') && (current->content)[len-1] != ' ')
		{
			
			size_t new_len = ft_strlen(current->content) + ft_strlen(next_nod->content) + 1;
			new_content = malloc(new_len);
			if (!new_content) 
				return;
			ft_strlcpy(new_content, current->content, new_len);
			ft_strlcat(new_content, next_nod->content, new_len);
			
			free(current->content);
			current->content = new_content;

			current->next = next_nod->next;
			if (next_nod->next)
                next_nod->next->prev = current;
			free(next_nod->content);
			free(next_nod);
			
		}
		current = current->next;
	}
}

void merge_quotes_nodes(t_list **head)
{
    if (!head || !*head) 
        return;

    t_list *current = *head;
    t_list *prv_nod;
	char *new_contentp;
    int lenp;

    while (current)
    {
        prv_nod = current->prev;
        
        if (prv_nod && prv_nod->content && current->content && (current->index == 0 && prv_nod->index == 0))
        {
            lenp = ft_strlen(prv_nod->content);
            
            if (lenp > 0 && (prv_nod->content[lenp-1] == '\"' || prv_nod->content[lenp-1] == '\'') && current->content[0] != ' ')
            {
                int new_len = ft_strlen(current->content) + lenp + 1;
                new_contentp = malloc(new_len);
                if (!new_contentp)
                    return;

                ft_strlcpy(new_contentp, prv_nod->content, lenp + 1);
                ft_strlcat(new_contentp, current->content, new_len);
                free(prv_nod->content);
                prv_nod->content = new_contentp;
                prv_nod->next = current->next;
                if (current->next)
                    current->next->prev = prv_nod;
                t_list *to_free = current;
                free(to_free->content);
                free(to_free);
                current = prv_nod;
                
            }
        }
        current = current->next;
    }
}