/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/06/24 07:07:40 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char * map_type(t_type_list type)
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
	t_tokenlist *current = head;
	while (current != NULL)
	{
		printf(BLUE"Content: --%s--, "YELLOW"type: %s \n"RESET, current->content, map_type(current->type));
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

int sintax_erreur(t_tokenlist *head)
{
	
	int i = 0;
	int j = 0;
	t_tokenlist *current = head;
	while (current != NULL)
	{
		if (current->type == comnd)
		{
			while (current->content[i] != '\0')
			{
				if (current->content[i] == ';' || current->content[i] == '\\')
					return (printf(RED"Syntax error: unexpected token '%c'\n"RESET, current->content[i]), ++j);
				i++;
			}
			i = 0;
			while (current->content[i] != '\0')
				i++;
			if( i==1 && (current->content[0]=='\'' || current->content[i-1]=='\"'))
				return (printf(RED"Syntax error: unclosed quotes\n"RESET), ++j);
			if((current->content[0]=='\'' && current->content[i-1]!='\'') ||
				(current->content[0]=='\"' && current->content[i-1]!='\"'))
				return (printf(RED"Syntax error: unclosed quotes\n"RESET), ++j);
		}
		if(current->type != comnd && (current->next == NULL || current->next->type != comnd))
			return (printf(RED"syntax error near unexpected token `newline'\n"RESET), ++j);
		if(current->type == PIPE && (current->prev == NULL || current->prev->type != comnd) )
			return (printf(RED"syntax error near unexpected token `|'\n"RESET), ++j);
		if(current->type == HEREdocument && (current->prev == NULL || current->prev->type != comnd))
			return (printf(RED"syntax error near unexpected token `newline'\n"RESET), ++j);
		current = current->next;
	}
	return j;
}

void merge_file_cmd(t_tokenlist **head) 
{
    if (!head || !*head) 
        return;
    t_tokenlist *current = *head;
    while (current) 
    {
        if ((current->type == INredirection && ft_strncmp(current->content, "<",1) == 0) ||
            (current->type == OUTredirection && ft_strncmp(current->content, ">",1) == 0) ||
            (current->type == HEREdocument && ft_strncmp(current->content, "<<",2) == 0) ||
            (current->type == OUTappend && ft_strncmp(current->content, ">>",2) == 0))
        {
            t_tokenlist *next_node = current->next;
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
    int j = i + 1;
    int k = 0;

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
    
    t_check *strackt = malloc(sizeof(t_check));
    if (!strackt) {
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
    int j = i;
    int k = 0;
    
    while (line[j] && line[j] != ' ' && line[j] != '|' && 
           line[j] != '<' && line[j] != '>')
        j++;

    cmd = malloc(j - i + 1);
    if (!cmd)
        return NULL;

    while (i < j)
        cmd[k++] = line[i++];
    cmd[k] = '\0';
    
    t_check *strackt = malloc(sizeof(t_check));
    if (!strackt) {
        free(cmd);
        return NULL;
    }
    
    strackt->str = ft_lstnewn(ft_strdup(cmd));
    strackt->i = j;
    free(cmd);
    return strackt;
}

int ssplit_line(char *line, t_tokenlist **head)
{
    int i = 0;
    
    while (line[i] != '\0')
    {
        while (line[i] == ' ' || line[i] == '\t')
            i++;
            
        if (line[i] == '\0')
            break;

        if (line[i] == '\'' || line[i] == '\"')
        {
            t_check *result = extract_cmd_quat(line, i, line[i]);
            if (!result)
                return printf(RED"Syntax error: unclosed quotes\n"RESET), 1;
                
            ft_lstadd_backn(head, result->str);
            i = result->i;
            free(result);
        }
        else if (line[i] == '|' || line[i] == '<' || line[i] == '>')
        {
            if (line[i] == '|') {
                ft_lstadd_backn(head, ft_lstnewn(ft_strdup("|")));
                i++;
            }
            else if (line[i] == '<' && line[i+1] == '<') {
                ft_lstadd_backn(head, ft_lstnewn(ft_strdup("<<")));
                i += 2;
            }
            else if (line[i] == '<') {
                ft_lstadd_backn(head, ft_lstnewn(ft_strdup("<")));
                i++;
            }
            else if (line[i] == '>' && line[i+1] == '>') {
                ft_lstadd_backn(head, ft_lstnewn(ft_strdup(">>")));
                i += 2;
            }
            else if (line[i] == '>') {
                ft_lstadd_backn(head, ft_lstnewn(ft_strdup(">")));
                i++;
            }
        }
        else
        {
            t_check *result = extract_cmd(line, i);
            if (!result)
                return printf(RED"Memory allocation error\n"RESET), 1;
                
            ft_lstadd_backn(head, result->str);
            i = result->i;
            free(result);
        }
    }
    return 0;
}

int main()
{
	t_tokenlist *head;
	char *s;
	int j;
	while (1)
	{
		head = NULL;  
		s = readline(GREEN"minishell> "RESET);
		if (!s)
			break;
		if (*s) 
		add_history(s);
		j = ssplit_line(s, &head);
		if(j==0)
		{
			j=sintax_erreur(head);
			if(j == 0)
			{
				merge_file_cmd(&head);
				print_nodes(head);
			}
		}
		free_list(head);
		free(s);
	}
	
	return 0;
}
