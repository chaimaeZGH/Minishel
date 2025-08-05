/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing_thrd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/08/05 18:36:39 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	three_lines(t_check *strackt, int i, int j, char *cmd)
{
	strackt->str = ft_lstnewn(ft_strdup(cmd));
	strackt->i = j + i;
	free(cmd);
}

t_check	*extract_cmd(char *line, int i)
{
	char	*cmd;
	t_check	*strackt;
	int		j;
	int		k;

	j = i;
	k = 0;
	while (line[j] && line[j] != ' ' && line[j] != '|' && 
		line[j] != '<' && line[j] != '>' && 
		line[j] != '\'' && line[j] != '\"')
		j++;
	cmd = malloc(j - i + 1);
	if (!cmd)
		return (NULL);
	while (i < j)
		cmd[k++] = line[i++];
	cmd[k] = '\0';
	strackt = malloc(sizeof(t_check));
	if (!strackt)
	{
		free(cmd);
		return (NULL);
	}
	three_lines(strackt, 0, j, cmd);
	return (strackt);
}

void	skip_whitespaces(int *i, char *line)
{
	while (line[*i] == ' ' || line[*i] == '\t')
		(*i)++;
}

void	merge_this_with_next(t_token **lst, char *ll)
{
	t_token	*current;
	t_token	*last;
	char	*new_content;

	if (!lst || !*lst || !ll)
		return ;
	current = *lst;
	while (current && current->content != ll)
		current = current->next;
	if (!current)
		return ;
	last = current->next;
	if (!last || current->type != 0 || last->type != 0)
		return ;
	new_content = NULL;
	add_new_conten(last, current, new_content);
	if (new_content == NULL)
		return ;
}

int	process_line_part(char *line, int *i, t_token **head, char **ll)
{
	skip_whitespaces(i, line);
	if (line[*i] == '\0')
		return (0);
	if (line[*i] == '\'' || line[*i] == '\"')
		return (handle_quotes(line, i, head, ll));
	else if (line[*i] == '|' || line[*i] == '<' || line[*i] == '>')
	{
		handle_operators(i, line, head);
		return (0);
	}
	return (handle_regular_cmd(line, i, head));
}
