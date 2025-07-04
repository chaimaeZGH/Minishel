/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/07/04 18:14:37 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int check_pipe (t_tokenlist *head)
{
    while (head != NULL)
    {
        if (head->type == PIPE)
            return(1);
        head = head->next;
    }
    return (0);
}

t_tree_list *create_tree(t_tokenlist **head)
{
    t_tree_list *tree;
    t_tokenlist *current = NULL;
    t_tokenlist *argm = NULL;
    t_tokenlist *outs = NULL;
    t_tokenlist *ins = NULL;
    t_tokenlist *last_arg = NULL;
    t_tokenlist *last_out = NULL;
    t_tokenlist *last_in = NULL;

    if (head == NULL || *head == NULL)
        return NULL;
    
    tree = malloc(sizeof(t_tree_list));
    if (tree == NULL)
        return NULL;
    
    t_cmdlist *cmd = malloc(sizeof(t_cmdlist));
    if (!cmd) 
    {
        free(tree);
        return NULL;a
    }
    cmd->here_doc_fd = -1;
    cmd->cmd = NULL;
    cmd->args = NULL;
    cmd->out = NULL;
    cmd->in = NULL;
    while (*head != NULL)
    {
        current = *head;
        *head = (*head)->next;
        current->next = NULL;
        current->prev = NULL;
        if (current->type == comnd)
        {
            if (argm == NULL) 
                argm = last_arg = current;
            else 
            {
                last_arg->next = current;
                current->prev = last_arg;
                last_arg = current;
            }
            printf("%u %s\n", current->type, current->content);
        }
        else if (current->type == INredirection || current->type == HEREdocument)
        {
            if (ins == NULL) 
                ins = last_in = current;
            else 
            {
                last_in->next = current;
                current->prev = last_in;
                last_in = current;
            }
            printf("%u %s\n",last_in->type, last_in->content);
        }
        else if (current->type == OUTappend || current->type == OUTredirection)
        {
            if (outs == NULL) 
                outs = last_out = current;
            else 
            {
                last_out->next = current;
                current->prev = last_out;
                last_out = current;
            }
            printf("%u %s\n",last_out->type, last_out->content);
        }
    }

    if (last_arg) last_arg->next = NULL;
    if (last_in) last_in->next = NULL;
    if (last_out) last_out->next = NULL;

    cmd->args = argm;
    cmd->in = ins;
    cmd->out = outs;

    tree->cmd = cmd;
    tree->left = NULL;
    tree->right = NULL;
    tree->type = comnd;
    
    return tree;
}

void fill_tree(t_tokenlist **head, t_tree_list *tree)
{
    int i = check_pipe (*head);
    if(i == 0)
        tree = create_tree(head);  
    // else
    // {
        // cut from pipe rooth recive the pipe
        // pointer that pont to the next of the pipe
        // create right for that pinter
        // recurseve to the left of what remain of the list
    // }
}
void print_tree(t_tree_list *tree, int a) // this is not working
{
    char *s;
    if (tree != NULL)
    {
        if(a == 0)
            s="rooth";
        if(a == 1)
            s="right";
        if(a == 2)
            s="left";
        printf("%s",s);
        t_tokenlist *tmp = tree->cmd->args;
        while (tmp != NULL)
        {
            printf("%s\n", tmp->content);
            tmp = tmp->next;
        }
        if(tree->right != NULL)
            print_tree(tree->right,1);
        if(tree->left != NULL)
            print_tree(tree->left,2);
    }
}
