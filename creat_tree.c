/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/07/15 17:43:45 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_cmd(char **cmd)
{
    int i = 0;
    if (!cmd)
        return;
    while (cmd[i] != NULL)
    {
        free(cmd[i]);
        i++;
    }
    free(cmd);
}

void free_s_cmd(t_cmdlist *cmd)
{
    if (!cmd)
        return;
    free_list(cmd->args);
    free_list(cmd->in);
    free_list(cmd->out);
    free_cmd(cmd->cmd);
    free(cmd);
}
void free_tree(t_tree_list *tree)
{
    if (!tree)
        return;
    if(tree->type != PIPE)
        free_s_cmd(tree->cmd);
    free_tree(tree->right);
    free_tree(tree->left);
    free(tree);
}

t_tokenlist *last_pipe(t_tokenlist *head)
{
    t_tokenlist *current;

    if (!head)
        return NULL;
    current = head;
    while (current->next != NULL)
        current = current->next;
    while(current != NULL)
    {
        if(current->type == PIPE)
            return (current);
        else
            current = current->prev;
    }
    return (current);
}

t_tokenlist *ft_lstnewnn(void *content, t_type_list ttype)
{
    t_tokenlist *head;

	head = malloc(sizeof(t_tokenlist));
	if (head == NULL)
		return NULL;
	head->content = content;
	head->next = NULL;
	head->prev = NULL;
	head->type = ttype;
    head->expnd = true;
	return head;
}

void fill_tree(t_cmdlist *cmd, t_tokenlist *original)
{
    t_tokenlist *head;

    if (original == NULL)
        return ;
    head = original;
    while(head != NULL)
    {
        if (head->type == comnd)
            ft_lstadd_backn(&cmd->args, ft_lstnewnn(ft_strdup(head->content),head->type));
        if (head->type == INredirection || head->type == HEREdocument)
            ft_lstadd_backn(&cmd->in, ft_lstnewnn(ft_strdup(head->content),head->type));
        if (head->type == OUTappend || head->type == OUTredirection)
            ft_lstadd_backn(&cmd->out, ft_lstnewnn(ft_strdup(head->content),head->type));
        head = head->next;
    }
}

t_tree_list *create_tree(t_tokenlist **head)
{
    t_tree_list *tree;
    t_tokenlist *last_p;

    if (head == NULL || *head == NULL)
        return NULL;

    // create_tree_node()
    tree = malloc(sizeof(t_tree_list));
    if (!tree)
        return NULL;
    
    tree->right = NULL;
    tree->left = NULL;
    tree->cmd = NULL;
    //

    last_p = last_pipe(*head);
    if (last_p == NULL)
    {
        // create_cmd_node()
        tree->cmd = malloc(sizeof(t_cmdlist));
        if (!tree->cmd)
        {
            free(tree);
            return NULL;
        }
        tree->cmd->here_doc_fd=-1;
        tree->cmd->cmd = NULL;
        tree->cmd->args = NULL;
        tree->cmd->in = NULL;
        tree->cmd->out = NULL;
        //

        tree->type = comnd;
        fill_tree(tree->cmd, *head);
        free_list(*head);  
    }
    else
    {
        //remove_node();
        t_tokenlist *previous;
        t_tokenlist *right_part;

        previous = last_p->prev;
        previous->next = NULL;
        right_part = last_p->next;
        right_part->prev = NULL;
        free(last_p->content);
        free(last_p);
        // 

        tree->type = PIPE;
        tree->right = create_tree(&right_part);
        tree->left = create_tree(head);
    }
    return tree;
}
