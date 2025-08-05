/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing_fif.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/08/05 18:35:45 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	merg_last_with_one(t_token **lst)
{
	t_token	*current;
	t_token	*prevvv;
	char	*new_content;

	if (!lst || !*lst || !(*lst)->next)
		return ;
	current = *lst;
	while (current->next != NULL)
		current = current->next;
	prevvv = current->prev;
	if (!prevvv)
		return ;
	if (current->type == 0 && prevvv->type == 0)
	{
		new_content = NULL;
		add_new_conten(current, prevvv, new_content);
		if (new_content == NULL)
			return ;
	}
}

void	handle_operators(int *i, char *line, t_token **head)
{
	if (line[*i] == '|') 
	{
		ft_lstadd_backn(head, ft_lstnewn(ft_strdup("|")));
		(*i)++;
	}
	else if (line[*i] == '<' && line[(*i) + 1] == '<')
	{
		ft_lstadd_backn(head, ft_lstnewn(ft_strdup("<<")));
		(*i) += 2;
	}
	else if (line[*i] == '<') 
	{
		ft_lstadd_backn(head, ft_lstnewn(ft_strdup("<")));
		(*i)++;
	}
	else if (line[*i] == '>' && line[(*i) + 1] == '>') 
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

int	handle_regular_cmd(char *line, int *i, t_token **head)
{
	t_check	*result;

	result = extract_cmd(line, *i);
	if (!result)
		return (printf("Memory allocation error\n"), 1);
	ft_lstadd_backn(head, result->str);
	*i = result->i;
	free(result);
	return (0);
}

t_check	*extract_cmd_quat(char *line, int i, char c)
{
	char	*cmd;
	t_check	*strackt;
	int		j;
	int		k;

	j = i + 1;
	k = 0;
	while (line[j] && line[j] != c)
		j++;
	if (line[j] == '\0')
		return (NULL);
	cmd = malloc(j - i + 2);
	if (!cmd)
		return (NULL);
	while (i <= j)
		cmd[k++] = line[i++];
	cmd[k] = '\0';
	strackt = malloc(sizeof(t_check));
	if (!strackt)
	{
		free(cmd);
		return (NULL);
	}
	three_lines(strackt, 1, j, cmd);
	return (strackt);
}

int	handle_quotes(char *line, int *i, t_token **head, char **ll)
{
	t_check	*result;
	int		j;

	j = *i;
	result = extract_cmd_quat(line, *i, line[*i]);
	if (!result)
		return (printf("Syntax error: unclosed quotes\n"), 1);
	ft_lstadd_backn(head, result->str);
	*i = result->i;
	free(result);
	if (j > 0 && line[j - 1] != ' ')
		merg_last_with_one(head);
	if (line[*i] != ' ' && line[*i] != '\0')
		*ll = (ft_lstlastn(*head))->content;
	return (0);
}
