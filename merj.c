/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merj.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/05/30 00:44:46 by czghoumi         ###   ########.fr       */
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
			if (!new_content) {
				perror("malloc failed");
				exit(EXIT_FAILURE);
			}
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