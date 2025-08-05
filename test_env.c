/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/08/04 23:20:41 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(char **env, char *name)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(name);
	while (env && env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

char	*ft_strndup(char *s, size_t n)
{
	size_t	len;
	char	*copy;

	len = 0;
	while (len < n && s[len] != '\0')
		len++;
	copy = malloc(len + 1);
	if (!copy)
		return (NULL);
	ft_memcpy(copy, s, len);
	copy[len] = '\0';
	return (copy);
}

char	*handle_special_cases(char *str, int *index, int exit_s)
{
	pid_t	pid;

	if ((str[*index + 1] == '\'' && str[*index - 1] == 2))
	{
		(*index) = (*index) + ft_strlen((char *)str);
		return (ft_strdup((char *)str));
	}
	if (str[*index + 1] == '\0')
		return (ft_strdup("$"));
	if (str[*index + 1] == '$')
	{
		(*index)++;
		pid = getpid();
		return (ft_itoa(pid));
	}
	if (str[*index + 1] == '?')
		return ((*index)++, ft_itoa(exit_s));
	if (str[*index + 1] == '0')
		return ((*index)++, ft_strdup("minishell"));
	if (ft_isdigit(str[*index + 1]))
		return ((*index)++, ft_strdup(""));
	return (NULL);
}

char	*expand_variable(char *str, int *index, char **env, int exit_s)
{
	int		i;
	char	*varname;
	char	*value;
	char	*result;

	result = handle_special_cases(str, index, exit_s);
	if (result)
		return (result);
	i = *index + 1;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	if (i == *index + 1)
		return (ft_strdup(""));
	varname = ft_strndup(str + *index + 1, i - (*index + 1));
	value = get_env_value(env, varname);
	free(varname);
	*index = i - 1;
	if (value)
		result = ft_strdup(value);
	else
		result = ft_strdup("");
	return (result);
}

int	append_char(char **result, char c)
{
	char	*new_result;
	size_t	len;

	len = ft_strlen(*result);
	new_result = malloc(len + 2);
	if (!new_result)
		return (0);
	ft_strlcpy(new_result, *result, len + 1);
	new_result[len] = c;
	new_result[len + 1] = '\0';
	free(*result);
	*result = new_result;
	return (1);
}
