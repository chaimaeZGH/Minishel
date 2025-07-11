/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rroundi <rroundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/07/08 20:17:25 by rroundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *map_type(t_type_list type)
{
	if (type == 0)
		return "command";
	if (type == 1)
		return "pipe";
	if (type == 2)
		return "redirection in";
	if (type == 3)
		return "redirection out";
	if (type == 4)
		return "redirection heredoc";
	if (type == 5)
		return "redirection append";
	return NULL;
}

void print_nodes(t_tokenlist *head)
{
    t_tokenlist *current;

	current = head;
	while (current != NULL)
	{
		printf(BLUE"Content: [%s], "YELLOW"type: %s \n"RESET, current->content, map_type(current->type));
		current = current->next;
	}
}

void free_list(t_tokenlist *head)
{
	t_tokenlist *tmp;
	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp->content);
		free(tmp);
	}
}


void    ft_error_msg(int errnum)
{
    if(errnum == 0)
        printf(RED"Syntax error: unexpected token ; or \\ \n"RESET);
     if(errnum == 1)
        printf(RED"syntax error near unexpected token \n"RESET);
    if(errnum == 2)
        printf(RED"syntax error near '|'\n"RESET);
    if(errnum == 3)
        printf(RED"syntax error near heardoc \n"RESET);
}

int syntax_erreur(t_tokenlist *head)
{
    int         i;
    int         j;
    t_tokenlist *current;

	i = 0;
	j = 0;
	current = head;
	while (current != NULL)
	{
		if (current->type == comnd)
		{
			while (current->content[i] != '\0')
			{
				if (current->content[i] == ';' || current->content[i] == '\\')
					return (ft_error_msg(0), ++j);
				i++;
			}
		}
		if(current->type != comnd && (current->next == NULL || current->next->type != comnd))
			return (ft_error_msg(1), ++j);
		if(current->type == PIPE && (current->prev == NULL || current->prev->type != comnd) )
			return (ft_error_msg(2), ++j);
		if(current->type == HEREdocument && (current->prev == NULL || current->prev->type != comnd))
			return (ft_error_msg(3), ++j);
		current = current->next;
	}
	return j;
}

void    merge_file_cmd(t_tokenlist **head) 
{
    t_tokenlist *current;
    t_tokenlist *next_node;

    if (!head || !*head) 
        return;
    current = *head;
    while (current) 
    {
        if ((current->type == INredirection && ft_strncmp(current->content, "<",1) == 0) ||
            (current->type == OUTredirection && ft_strncmp(current->content, ">",1) == 0) ||
            (current->type == HEREdocument && ft_strncmp(current->content, "<<",2) == 0) ||
            (current->type == OUTappend && ft_strncmp(current->content, ">>",2) == 0))
        {
            next_node = current->next;
            if (next_node) 
			{
                next_node->type = current->type;
                if (current->prev)
                    current->prev->next = next_node;
                else
                    *head = next_node;
                next_node->prev = current->prev;
                free(current->content);
                free(current);
                current = next_node;
                continue;
            }
        }
        current = current->next;
    }
}

t_check *extract_cmd_quat(char *line, int i, char c)
{
    char *cmd;
    t_check *strackt;
    int j;
    int k;

    j = i + 1;
    k = 0;
    while (line[j] && line[j] != c)
        j++;
    if (line[j] == '\0')
        return NULL;
    cmd = malloc(j - i + 2);
    if (!cmd)
        return NULL;
    while (i <= j)
        cmd[k++] = line[i++];
    cmd[k] = '\0';
    strackt = malloc(sizeof(t_check));
    if (!strackt) 
    {
        free(cmd);
        return NULL;
    }
    strackt->str = ft_lstnewn(ft_strdup(cmd));
    strackt->i = j + 1; 
    free(cmd);
   
    return strackt;
}

t_check *extract_cmd(char *line, int i)
{
    char *cmd;
    t_check *strackt;
    int j;
    int k;
 
    j = i;
    k = 0;
    while (line[j] && line[j] != ' ' && line[j] != '|' && 
           line[j] != '<' && line[j] != '>' && line[j] != '\'' && line[j] != '\"')
        j++;
    cmd = malloc(j - i + 1);
    if (!cmd)
        return NULL;
    while (i < j)
        cmd[k++] = line[i++];
    cmd[k] = '\0';
    strackt = malloc(sizeof(t_check));
    if (!strackt) 
    {
        free(cmd);
        return NULL;
    }
    strackt->str = ft_lstnewn(ft_strdup(cmd));
    strackt->i = j;
    free(cmd);
    return strackt;
}

void    skip_whitespaces(int *i, char *line)
{
    while (line[*i] == ' ' || line[*i] == '\t')
        (*i)++;
}
void merge_this_with_next(t_tokenlist **lst, char *ll)
{
    t_tokenlist *current;
    t_tokenlist *last;
    size_t new_len;
    char *new_content;

    if (!lst || !*lst || !ll)
        return;

    current = *lst;
    while (current && current->content != ll)
        current = current->next;

    if (!current)
        return;

    last = current->next;
    if (!last || current->type != 0 || last->type != 0)
        return;

    new_len = ft_strlen(current->content) + ft_strlen(last->content) + 1;
    new_content = malloc(new_len);
    if (!new_content)
        return;

    ft_strlcpy(new_content, current->content, new_len);
    ft_strlcat(new_content, last->content, new_len);

    free(current->content);
    current->content = new_content;

    current->next = last->next;
    if (last->next)
        last->next->prev = current;

    free(last->content);
    free(last);
}


void merg_last_with_one(t_tokenlist **lst)
{
    t_tokenlist *current;
    t_tokenlist *prevvv;
    size_t new_len;
    char *new_content;

    if (!lst || !*lst || !(*lst)->next)
        return;
    current = *lst;
    while (current->next != NULL)
        current = current->next;
    prevvv = current->prev;
    if (!prevvv)
        return;
    if (current->type==0 && prevvv->type==0)
    {
        new_len = ft_strlen(current->content) + ft_strlen(prevvv->content) + 1;
        new_content = malloc(new_len);
        if (!new_content)
            return;
        ft_strlcpy(new_content, prevvv->content, new_len);
        ft_strlcat(new_content, current->content, new_len);
        free(prevvv->content);
        prevvv->content = new_content;
        prevvv->next = current->next;
        if (current->next)
            current->next->prev = prevvv;
        free(current->content);
        free(current);
    }
}

void    handle_operators(int *i, char *line, t_tokenlist **head)
{
    if (line[*i] == '|') 
    {
        ft_lstadd_backn(head, ft_lstnewn(ft_strdup("|")));
        (*i)++;
    }
    else if (line[*i] == '<' && line[(*i)+1] == '<')
        {
        ft_lstadd_backn(head, ft_lstnewn(ft_strdup("<<")));
        (*i) += 2;
    }
    else if (line[*i] == '<') 
    {
        ft_lstadd_backn(head, ft_lstnewn(ft_strdup("<")));
        (*i)++;
    }
    else if (line[*i] == '>' && line[(*i)+1] == '>') 
    {
        ft_lstadd_backn(head, ft_lstnewn(ft_strdup(">>")));
        (*i) += 2;
    }
    else if (line[*i] == '>') 
    {
        ft_lstadd_backn(head, ft_lstnewn(ft_strdup(">")));
        (*i)++;
    }
}

int split_line(char *line, t_tokenlist **head)
{
    t_check *result;
    int     i;
    int     j;
    char *ll;//
    
    ll = NULL;
    i = 0;
    while (line[i] != '\0')
    {
        skip_whitespaces(&i, line);
        if (line[i] == '\0')
            break ;
        if (line[i] == '\'' || line[i] == '\"')
        {
            j = i;
             result = extract_cmd_quat(line, i, line[i]);
            if (!result)
                return printf(RED"Syntax error: unclosed quotes\n"RESET), 1;
            ft_lstadd_backn(head, result->str);
            i = result->i;
            
            free(result);
            if(line[j-1] != ' ' && j > 0)
                merg_last_with_one(head);
            if(line[i] != ' ' && line[i] != '\0')
                ll=ft_lstlastn(*head)->content;
        }
        else if (line[i] == '|' || line[i] == '<' || line[i] == '>')
            handle_operators(&i, line, head);
        else
        {
            result = extract_cmd(line, i);
            if (!result)
                return printf(RED"Memory allocation error\n"RESET), 1;
            ft_lstadd_backn(head, result->str);
            i = result->i;
            free(result);
        }
        if(ll != NULL)
            merge_this_with_next(head,ll);
    }
    return 0;
}
// void replace_expend(t_tree_list *tree)
// {
    
// }
void fill_cmd_from_args(t_cmdlist *cmd)
{
    if (!cmd || !cmd->args)
    {
        cmd->cmd = NULL;
        return;
    }
    int count = 0;
    t_tokenlist *current = cmd->args;
    while (current)
    {
        count++;
        current = current->next;
    }
    cmd->cmd = (char **)malloc(sizeof(char *)*(count + 1));
    if (!cmd->cmd)
        return;
    current = cmd->args;
    int i = 0;
    while (current)
    {
        cmd->cmd[i] = strdup(current->content);
        if (!cmd->cmd[i])
        {
            while (i > 0)
                free(cmd->cmd[--i]);
            free(cmd->cmd);
            cmd->cmd = NULL;
            return;
        }
        i++;
        current = current->next;
    }
    cmd->cmd[count] = NULL;
    // int j = 0;
    // while(cmd->cmd[j]!=NULL)
    // {
    //     printf("%s\n",cmd->cmd[j]);
    //     j++;
    // }
}

void fill_double_point(t_tree_list *tree)
{
    if (!tree)
        return;
    if (tree->type == comnd)
        fill_cmd_from_args(tree->cmd);
    fill_double_point(tree->left);
    fill_double_point(tree->right);
}

t_tree_list     *init_shell(char *s)
{
    t_tokenlist *head;
    t_tree_list *tree;

    head = NULL;
    tree = NULL;
    if(split_line(s,&head) == 0)
	{   
	    if(syntax_erreur(head) == 0)
	    {
		    merge_file_cmd(&head);
            tree = create_tree(&head);
            fill_double_point(tree);
            // replace_expend(tree);//
            // print_ast_topdown(tree);
            // print_nodes(head);//
	    }
        else
            free_list(head);
	}
    else
        free_list(head);
    free(s);
    return (tree);
}

int main(int argc, char **argv, char **envp)
{
    
	char        *s;
    t_tree_list *tree;
    t_env   *env;
    (void)envp;
    (void)argv;
        env = copy_env(envp);
    if (argc != 1)
        return (printf(RED"invalid number of arguments\nUsage: ./minishell\n"RESET), 1);
	while (1)
	{
		s = readline(GREEN"minishell> "RESET);
		if (!s)
			break;
		if (*s) 
		    add_history(s);
        tree = init_shell(s);
        if(execute(tree, &env) == 1)
        {
            free_tree(tree);
            return (1);
        }
    }

    free_tree(tree);
    return (0);
}
