/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/07/06 01:30:52 by czghoumi         ###   ########.fr       */
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
    free_s_cmd(tree->cmd);
    free_tree(tree->right);
    free_tree(tree->left);
    free(tree);
}

t_tokenlist *last_pipe(t_tokenlist *head)
{
    if (!head)
        return NULL;
    t_tokenlist *current = head;

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
	t_tokenlist *head = malloc(sizeof(t_tokenlist));
	if (head == NULL)
		return NULL;
		
	head->content = content;
	head->next = NULL;
	head->prev = NULL;
	head->type = ttype;
	return head;
}

void fill_tree(t_cmdlist *cmd, t_tokenlist *original)
{
    if (original == NULL)
        return ;
    t_tokenlist *head = original;
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
    free_list(original);
}

t_tree_list *create_tree(t_tokenlist **head)
{
    if (head == NULL || *head == NULL)
        return NULL;

    t_tree_list *tree = malloc(sizeof(t_tree_list));
    if (!tree)
        return NULL;

    tree->cmd = malloc(sizeof(t_cmdlist));
    if (!tree->cmd)
    {
        free(tree);
        return NULL;
    }
    
    t_tokenlist *last_p = last_pipe(*head);
    if (last_p == NULL)
    {
        tree->right = NULL;
        tree->left = NULL;
        tree->type = comnd;
        tree->cmd->here_doc_fd=-1;
        tree->cmd->cmd = NULL;
        tree->cmd->args = NULL;
        tree->cmd->in = NULL;
        tree->cmd->out = NULL;
        fill_tree(tree->cmd, *head);
    }
    else
    {
        tree->type = PIPE;
        tree->cmd = NULL;
        t_tokenlist *privius = last_p->prev;
        privius->next = NULL;
        t_tokenlist *right_part = last_p->next;
        right_part->prev = NULL;
        free(last_p->content);
        free(last_p);
        tree->right = create_tree(&right_part);
        tree->left = create_tree(head);
    }
    return tree;
}