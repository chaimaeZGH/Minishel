/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing_sev.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/08/05 19:02:25 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == 1 || str[i] == 2)
			return (1);
		i++;
	}
	return (0);
}

int	initial_new_list(t_token *tkn, char **ev, t_token **lt, int et_s)
{
	t_token	*new;
	char	*cont;

	while (tkn)
	{
		cont = expand_content(tkn->content, ev, et_s);
		if ((ft_strlen(tkn->content) != 0 && ft_strlen(cont) == 0))
			free(cont);
		else
		{
			new = ft_lstnewnn(cont, tkn->type);
			if (!new)
				return (free(cont), 0);
			if (ft_strchr(tkn->content, '$') != NULL && 
				ft_strchr(cont, ' ') != NULL)
				new->expnd = false;
			else
				new->expnd = true;
			ft_lstadd_backn(lt, new);
		}
		tkn = tkn->next;
	}
	return (1);
}

t_token	*handle_args(t_token *token, char **env, int exit_s )
{
	t_token	*list;
	t_token	*tmp;

	list = NULL;
	tmp = token;
	if (!initial_new_list(token, env, &list, exit_s))
	{
		free_list(list);
		free_list(tmp);
		return (NULL);
	}
	free_list(tmp);
	return (list);
}

void	check_expend_heardoc(t_token *current_in, char **env, int exit_s)
{
	t_token	*next;

	while (current_in)
	{
		if (current_in->type == HEREdocument)
		{
			if (check_quotes(current_in->content) == 1)
				current_in->expnd = false;
		}
		next = current_in->next;
		if (current_in->type != HEREdocument)
			process_expend_content(current_in, env, exit_s);
		current_in = next;
	}
}

void	expend_check(t_tree *root, char **env, int exit_s)
{
	t_token	*current_in;
	t_token	*current_out;
	t_token	*next;

	if (root->type == comnd && root->cmd)
	{
		current_in = root->cmd->in;
		current_out = root->cmd->out;
		root->cmd->args = handle_args(root->cmd->args, env, exit_s);
		check_expend_heardoc(current_in, env, exit_s);
		while (current_out)
		{
			next = current_out->next;
			process_expend_content(current_out, env, exit_s);
			current_out = next;
		}
	}
}
