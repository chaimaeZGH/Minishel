/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tst_env_thrd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/08/05 18:51:23 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_st	*init_env_ex(char **env, int exit_s)
{
	t_st	*env_ex;

	env_ex = malloc(sizeof(t_st));
	if (!env_ex)
		return (NULL);
	env_ex->i = exit_s;
	env_ex->str = env;
	return (env_ex);
}

static t_char	*init_strac(char *content, char *quote_char)
{
	t_char	*strac;

	strac = malloc(sizeof(t_char));
	if (!strac)
		return (NULL);
	strac->str = content;
	strac->one = quote_char;
	return (strac);
}

static int	process_loop(char *content, t_st *env_ex,
				t_char *strac, char **result)
{
	int	i;

	i = 0;
	while (content[i])
	{
		if (!process_char(strac, &i, result, env_ex))
			return (0);
	}
	return (1);
}

char	*expnd_cont(char *content, char **env, int exit_s)
{
	t_st		*env_ex;
	t_char		*strac;
	char		*result;
	char		quote_char;

	env_ex = init_env_ex(env, exit_s);
	if (!env_ex)
		return (NULL);
	quote_char = 0;
	strac = init_strac(content, &quote_char);
	if (!strac)
		return (free(env_ex), NULL);
	result = malloc(1);
	if (!result)
		return (free(strac), free(env_ex), NULL);
	result[0] = '\0';
	if (!process_loop(content, env_ex, strac, &result))
		return (free(result), free(strac), free(env_ex), NULL);
	free(strac);
	free(env_ex);
	return (result);
}
