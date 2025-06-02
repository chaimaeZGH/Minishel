/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/06/02 21:49:12 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char *ft_trimm(char *str)
{
    int i = 0;
    int k = 0;
    int new_size;
    char *new_str;
    int j = ft_strlen(str) - 1;

    while (str[i] == ' ' || str[i] == '\t')
        i++;
    while (j >= i && (str[j] == ' ' || str[j] == '\t'))
        j--;
    new_size = j - i + 1;
    new_str = malloc(new_size + 1);
    if (!new_str)
        return NULL;
    
    while (i <= j)
        new_str[k++] = str[i++];
    new_str[k] = '\0';
    return new_str;
}

char	*ft_strdup(const char *s1)
{
	char	*ptr;
	int		i;

	ptr = malloc(ft_strlen((char *)s1) + 1);
	if (ptr == NULL)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		ptr[i] = s1[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;
	int		def;

	i = 0;
	def = 0;
	while (((unsigned char)s1[i] || (unsigned char)s2[i]) && i < n)
	{
		def = (unsigned char)s1[i] - (unsigned char)s2[i];
		if (def != 0)
            return def;
        i++;
	}
	return (0);
}

t_list *ft_lstnew(void *content)
{
    t_list *head = malloc(sizeof(t_list));
    if (head == NULL)
        return NULL;
        
    head->content = content;
    head->next = NULL;
    head->prev = NULL;
    
    if (content == NULL) {
        head->index = comnd;
        return head;
    }
    
    if (ft_strncmp("|", (char *)content, 1) == 0)
        head->index = PIPE;
    else if (ft_strncmp("<<", (char *)content, 2) == 0)
        head->index = HEREdocument;
    else if (ft_strncmp(">>", (char *)content, 2) == 0)
        head->index = OUTappend;
    else if (ft_strncmp("<", (char *)content, 1) == 0)
        head->index = INredirection;
    else if (ft_strncmp(">", (char *)content, 1) == 0)
        head->index = OUTredirection;
    else if (ft_strncmp("\"", (char *)content, 1) == 0)
        head->index = comnd;
    else if (ft_strncmp("\'", (char *)content, 1) == 0)
        head->index = comnd;
    else
        head->index = comnd;
        
    return head;
}

t_list	*ft_lstlast(t_list *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
	{
		lst = lst->next;
	}
	return (lst);
}

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*l;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	l = ft_lstlast(*lst);
	l->next = new;
    new->prev = l;
}

t_list  *extract_command(char *str, int start, int end)
{
    if (start >= end)
        return NULL;
        
    char *cmd = malloc(end - start + 1);
    if (!cmd)
        return NULL;
        
    int j = 0;
    while (start < end)
    {
        cmd[j] = str[start];
        start++;
        j++;
    }
    cmd[j] = '\0';
    if (cmd[0] == '\0') 
    {
        free(cmd);
        return NULL;
    }
    t_list *node = ft_lstnew(cmd);
    if (node == NULL)
    {
        free(cmd);
        return NULL;
    }
    return node;
}

void    split_line(char *line,int i, t_list **head)
{   
    int start = i;
    while(line[i] != '|' && line[i] != '<' && line[i] != '>' && line[i] != '\0' && line[i] != '\"' && line[i] != '\'' ) 
        i++;
    t_list *node = extract_command(line, start, i);
    if (node != NULL)
    ft_lstadd_back(head, node);

    if (line[i] == '|')
    {
        node = ft_lstnew(ft_strdup("|"));
        ft_lstadd_back(head, node);
        i++;
    }
    else if (line[i] == '<')
    {
        if(line[i + 1] == '<')
        {
            node= ft_lstnew(ft_strdup("<<"));
            ft_lstadd_back(head, node);
            i += 2;
        }
        else
        {
            node= ft_lstnew(ft_strdup("<"));
            ft_lstadd_back(head, node);
            i++;
        }
    }
    else if (line[i] == '>')
    {
        if(line[i + 1] == '>')
        {
            node= ft_lstnew(ft_strdup(">>"));
            ft_lstadd_back(head, node);
            i += 2;
        }
        else
        {
            node= ft_lstnew(ft_strdup(">"));
            ft_lstadd_back(head, node);
            i++;
        }
    }
    else if (line[i] == '\"')
    {
            node = ft_lstnew(ft_strdup("\""));
            ft_lstadd_back(head, node);
            i++;
    }
    else if (line[i] == '\'')
    {
            node = ft_lstnew(ft_strdup("\'"));
            ft_lstadd_back(head, node);
            i++;
    }

    if (line[i] != '\0')
        split_line(line,i, head);
}

char * map_type(type_list index)
{
    if (index == 0)
        return "command";
    if (index == 1)
        return "pipe";
    if (index == 2)
        return "redirection in";
    if (index == 3)
        return "redirection out";
    if (index == 4)
        return "redirection heredoc";
    if (index == 5)
        return "redirection append";
    return NULL;
}

void print_nodes(t_list *head)
{
    t_list *current = head;
    while (current != NULL)
    {
        printf(BLUE"Content: --%s--, "YELLOW"type: %s \n"RESET, current->content, map_type(current->index));
        current = current->next;
    }
}

void free_list(t_list *head)
{
    t_list *tmp;
    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp->content);
        free(tmp);
    }
}

char *delet_spaces(char *str)
{
    int length = 0;
    int i = 0;
    while(str[i] != '\0')
    {
        if (str[i] != ' ' && str[i] != '\t')
        {
            length++;
            i++;
        }
        else if ((str[i] == ' ' || str[i] == '\t') && (length > 0 && str[i-1] != ' ' && str[i-1] != '\t'))
        {
            length++;
            i++;
        }
        else
        {
            i++;
        }
    }
    
    char *new_str = (char *)malloc(length + 1);
    if (new_str == NULL) 
    {
        return NULL;
    }
    
    i = 0;
    int j = 0;
    while(str[i] != '\0')
    {
        if (str[i] != ' ' && str[i] != '\t')
        {
            new_str[j] = str[i];
            j++;
            i++;
        }
        else if ((str[i] == ' ' || str[i] == '\t') && (j > 0 && new_str[j-1] != ' ' && new_str[j-1] != '\t'))
        {
            new_str[j] = ' ';
            j++;
            i++;
        }
        else
        {
            i++;
        }
    }
    new_str[j] = '\0';
    free(str);
    return new_str;
}

int chack_white_space(char *str)
{
    int i = 0 ;
    while(str[i])
    {
        if (str[i]==' ')
            i++;
        else 
            return 0;
    }
    return 1;
}
void clear_spaces(t_list **head)
{
    char *ptr;
    t_list *curent= *head;
    while (curent!=NULL)
    {
        if(curent->index == comnd && (curent->content[0] != '\'' && curent->content[0] != '\"') && chack_white_space(curent->content) != 1)
        {
            ptr = curent->content;
            ptr = delet_spaces(ptr);
            curent->content = ptr;
        }
        curent = curent->next;
    }
}

int sintax_erreur(t_list *head)
{
    
    int i = 0;
    int j = 0;
    t_list *current = head;
    while (current != NULL)
    {
        if (current->index == comnd)
        {
            while (current->content[i] != '\0')
            {
                if (current->content[i] == ';' || current->content[i] == '\\')
                {
                    printf(RED"Syntax error: unexpected token '%c'\n"RESET, current->content[i]);
                    j = 1;
                }
                i++;
            }
            i = 0;
            while (current->content[i] != '\0')
            
                i++;
            if( i==1 && (current->content[0]=='\'' || current->content[i-1]=='\"'))
            {
                printf(RED"Syntax error: unclosed quotes\n"RESET);
                j= 1;
            }
        
            if((current->content[0]=='\'' && current->content[i-1]!='\'') ||
                (current->content[0]=='\"' && current->content[i-1]!='\"'))
            {
                printf(RED"Syntax error: unclosed quotes\n"RESET);
                j = 1;
            }
        }
        if(current->index != comnd && (current->next == NULL || current->next->index != comnd))
        {
            printf(RED"syntax error near unexpected token `newline'\n"RESET);
            j = 1;
        }
        if(current->index == PIPE && (current->prev == NULL || current->prev->index != comnd) )
        {
            printf(RED"syntax error near unexpected token `|'\n"RESET);
            j = 1;
        }
        if(current->index == HEREdocument && (current->prev == NULL || current->prev->index != comnd))
        {
            printf(RED"syntax error near unexpected token `newline'\n"RESET);
            j = 1;
        }
        current = current->next;
    }
    return j;
}

void trimm_white_nods(t_list **head)
{
    if (!head || !*head) 
		return;
    t_list *current;
    current = *head;
    while (current != NULL)
	{
        if (current->index == 0 && chack_white_space(current->content) == 1)
        {
            free(current->content);
            current->content = malloc(2);
            current->content[0] = ' ';
            current->content[1] = '\0';
        }
        current = current->next;
    }
}

void merge_file_cmd(t_list **head) 
{
    if (!head || !*head) 
        return;
    
    t_list *current = *head;
    while (current) 
    {
        t_list *prev_node = current->prev;

        if (prev_node && prev_node->index && current->index == 0 
            && (prev_node->index >= 2 && prev_node->index <= 5)) 
        {
            current->index = prev_node->index;
            
            if (prev_node->prev)
                prev_node->prev->next = current;
            else
                *head = current;
            current->prev = prev_node->prev;
            free(prev_node->content);
            free(prev_node);
        }
        current = current->next;
    }
}

// int main(int argc, char **argv, char **env) till working with envirement
int main()
{
    char *ptr;
    t_list *head;
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
        ptr = ft_trimm(s);
        if (ptr != NULL && *ptr != '\0') 
        {
            split_line(ptr, 0, &head);
            what_to_merge(&head);
            j = sintax_erreur(head);
            if(j == 0)
            {
                merge_cmd_quoat(&head);
                clear_spaces(&head);
                trimm_white_nods(&head);
                merge_quotes_nodes(&head);
                merge_file_cmd(&head);
                print_nodes(head);
                // printf("Syntax is correct creat tree.\n");
            }
            free_list(head);
        }
        free(ptr);
        free(s);
    }
    
    return 0;
}
