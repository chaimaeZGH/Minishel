/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:59:01 by czghoumi          #+#    #+#             */
/*   Updated: 2025/05/29 23:12:05 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h> 


typedef enum
{
    comnd,
	PIPE,
    INredirection,
    OUTredirection,
    HEREdocument,
    OUTappend
}	type_list;

typedef struct s_list
{
	char			*content;
	struct s_list	*prev;
	struct s_list	*next;
	type_list		index;
}	t_list;

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
void merge_quotes(t_list *start, t_list *end);
void what_to_merge(t_list **head);
size_t	ft_strlen(const char *str);


#endif