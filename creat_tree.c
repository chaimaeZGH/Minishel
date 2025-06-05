/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/06/05 14:48:45 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree_list *create_node(void *content)
{
    t_tree_list *brnch = malloc(sizeof(t_tree_list));
	if (brnch == NULL)
		return NULL;
		
	brnch->right = content;
	brnch->left = NULL;
	brnch->type = NULL;
    brnch->cmd=NULL;
	return brnch;
}

