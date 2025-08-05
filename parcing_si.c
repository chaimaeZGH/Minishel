/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing_si.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/08/05 19:02:25 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	split_line(char *line, t_token **head)
{
	int		i;
	char	*ll;

	i = 0;
	ll = NULL;
	while (line[i] != '\0')
	{
		if (process_line_part(line, &i, head, &ll) != 0)
			return (1);
		else
		{
			if (ll != NULL)
				merge_this_with_next(head, ll);
		}
	}
	return (0);
}

void	change_quots(t_combo *new, int j, char quote_char)
{
	while (new->str[new->i] != '\0')
	{
		if ((new->str[new->i] == '\'' || new->str[new->i] == '\"') && j < 2)
		{
			if (j == 0)
			{
				quote_char = new->str[new->i];
				if (quote_char == '\'')
					new->str[new->i] = 1;
				else
					new->str[new->i] = 2;
				j = 1;
			}
			else if (new->str[new->i] == quote_char)
			{
				if (quote_char == '\'')
					new->str[new->i] = 1;
				else
					new->str[new->i] = 2;
				j = 0;
			}
		}
		(new->i)++;
	}
}

void	process_token_content(t_token *token)
{
	int		j;
	char	quote_char;
	t_combo	*new_cont;
	char	*tmp;

	if (!token || !token->content)
		return ;
	new_cont = malloc(sizeof(t_combo));
	if (!new_cont)
		return ;
	new_cont->str = strdup(token->content);
	if (!new_cont->str)
		return ;
	new_cont->i = 0;
	j = 0;
	quote_char = 0;
	change_quots(new_cont, j, quote_char);
	tmp = new_cont->str;
	free(token->content);
	token->content = tmp;
	free(new_cont);
}

void	replace_eache_node(t_tree *root)
{
	t_token	*current_arg;
	t_token	*current_in;
	t_token	*current_out;

	if (root->type == comnd && root->cmd)
	{
		current_arg = root->cmd->args;
		current_in = root->cmd->in;
		current_out = root->cmd->out;
		while (current_arg)
		{
			process_token_content(current_arg);
			current_arg = current_arg->next;
		}
		while (current_in)
		{
			process_token_content(current_in);
			current_in = current_in->next;
		}
		while (current_out)
		{
			process_token_content(current_out);
			current_out = current_out->next;
		}
	}
}

void	replace_quotes(t_tree *root)
{
	if (!root)
		return ;
	replace_eache_node(root);
	replace_quotes(root->right);
	replace_quotes(root->left);
}
