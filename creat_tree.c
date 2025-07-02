/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/07/03 00:05:48 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_pipe (t_tokenlist *head)
{
    while (head != NULL)
    {
        if (head->type=PIPE)
            return(1);
        head = head->next;
    }
    return (0);
}

void fill_tree(t_tokenlist **head,t_tree_list *tree)
{
    t_tokenlist *current = *head;
    int i = check_pipe (head);
    if(i == 0)
    {
        
    }
    else
    {
        
    }
}






