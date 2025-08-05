/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing_egh.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/08/05 19:02:25 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_for_expend(t_tree *root, char **env, int exit_s)
{
	if (!root)
		return ;
	expend_check(root, env, exit_s);
	check_for_expend(root->right, env, exit_s);
	check_for_expend(root->left, env, exit_s);
}

void	check_string(char *old, int *i, int *j)
{
	while (old[*i] != '\0')
	{
		if (old[*i] != 1 && old[*i] != 2)
			(*j)++;
		(*i)++;
	}
}

void	fill_string(char *old, char *new)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (old[i] != '\0')
	{
		if (old[i] != 1 && old[i] != 2)
		{
			new[j] = old[i];
			j++;
		}
		i++;
	}
	new[j] = '\0';
}

void	deleet_quots(t_token *str)
{
	char	*new;
	char	*old;
	int		i;
	int		j;

	j = 0;
	i = 0;
	old = str->content;
	check_string(old, &i, &j);
	new = malloc(j + 1);
	if (!new)
		return ;
	fill_string(old, new);
	free(str->content);
	str->content = new;
}

void	call_remove_quotes(t_token *current)
{
	while (current)
	{
		if (current->content)
			deleet_quots(current);
		current = current->next;
	}
}
