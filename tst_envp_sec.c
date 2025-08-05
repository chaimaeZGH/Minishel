/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tst_envp_sec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/08/05 18:50:48 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_var(char *content, int *i, char **result, t_st *env_ex)
{
	char	*expanded;
	char	*new_result;
	size_t	len_result;
	size_t	len_exp;

	len_result = ft_strlen(*result);
	expanded = expand_variable(content, i, env_ex->str, env_ex->i);
	if (!expanded)
		return (0);
	len_exp = ft_strlen(expanded);
	new_result = malloc(len_result + len_exp + 1);
	if (!new_result)
	{
		free(expanded);
		return (0);
	}
	ft_strlcpy(new_result, *result, len_result + 1);
	ft_strlcat(new_result, expanded, len_result + len_exp + 1);
	free(*result);
	free(expanded);
	*result = new_result;
	return (1);
}

int	handle_quotes_expend(char c, char *quote_char, int *i)
{
	if (c == 1 || c == 2)
	{
		if (*quote_char == 0)
			*quote_char = c;
		else if (*quote_char == c)
			*quote_char = 0;
		(*i)++;
		return (1);
	}
	return (0);
}

int	process_char(t_char *strac, int *i, char **result, t_st	*env_ex)
{
	char	c;

	c = strac->str[*i];
	if (handle_quotes_expend(c, strac->one, i))
		return (1);
	if (c == '$' && *strac->one != 1)
	{
		if (!expand_var(strac->str, i, result, env_ex))
			return (0);
	}
	else
	{
		if (!append_char(result, c))
			return (0);
	}
	(*i)++;
	return (1);
}

char	*expand_content(char *content, char **env, int exit_s)
{
	char	*result;
	int		i;

	i = 0;
	if (ft_strlen(content) == 2 && (content[0] == 1 || content[0] == 2))
	{
		result = malloc(3);
		while (content[i] != '\0')
		{
			result[i] = content[i];
			i++;
		}
		result[i] = '\0';
	}
	else
		result = expnd_cont(content, env, exit_s);
	return (result);
}
