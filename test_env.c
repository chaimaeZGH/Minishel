
#include "minishell.h"

char	*get_env_value(char **env, const char *name)
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

char	*ft_strndup(const char *s, size_t n)
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

static char	*handle_special_cases(const char *str, int *index, int exit_s)
{
	if	(str[*index + 1] == '\0')
		return (ft_strdup("$"));
	if	(str[*index + 1] == '$')
	{
		(*index)++;
		pid_t pid = getpid();
		return (ft_itoa(pid));
	}
	if (str[*index + 1] == '?')
	{
		(*index)++;
		return (ft_itoa(exit_s));
	}
	if (str[*index + 1] == '0')
	{
		(*index)++;
		return (ft_strdup("minishell"));
	}
	if (ft_isdigit(str[*index + 1]))
	{
		(*index)++;
		return (ft_strdup(""));
	}
	return (NULL);
}

char	*expand_variable(const char *str, int *index, char **env, int exit_s)
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

int	expand_var(const char *content, int *i, char **result, char **env, int exit_s)
{
	char	*expanded;
	char	*new_result;
	size_t	len_result;
	size_t	len_exp;

	len_result = ft_strlen(*result);
	expanded = expand_variable(content, i, env, exit_s);
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

int	process_char(t_char *strac, int *i, char **result, char **env, int exit_s)
{
	char	c;

	c = strac->str[*i];
	if (handle_quotes_expend(c, strac->one, i))
		return (1);
	if (c == '$' && *strac->one != 1)
	{
		if (!expand_var(strac->str, i, result, env, exit_s))
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

char	*expnd_cont(const char *content, char **env, int exit_s)
{
	char	*result;
	char	quote_char;
	int		i;
	t_char	*strac;
	
	i = 0;
	strac = malloc(sizeof(t_char));
	if (!strac)
		return (NULL);
	result = malloc(1);
	if (!result)
		return (free(strac), NULL);
	result[0] = '\0';
	quote_char = 0;
	strac->str = content;
	strac->one = &quote_char;
	while (content[i])
	{
		if (!process_char(strac, &i, &result, env, exit_s))
			return (free(result), free(strac), NULL);
	}
	free(strac);
	return (result);
}

char	*expand_content(const char *content, char **env, int exit_s)
{
	char	*result;
	int		i;
	
	i = 0;
	if(ft_strlen(content) == 2 &&(content[0] == 1 || content[0] == 2))
	{
		result = malloc(3);
		while(content[i]!='\0')
		{
			result[i]=content[i];
			i++;
		}
		result[i]='\0';
	}
	else
		result = expnd_cont(content, env, exit_s);
	return (result);
}

void	process_expend_content(t_tokenlist *token, char **env, int exit_s)
{
	char	*new_content;

	if (!token || !token->content)
		return ;
	new_content = expand_content(token->content, env, exit_s);
	if(ft_strchr(token->content,'$') != NULL && ft_strchr(new_content,' ')!=NULL)
		token->expnd = false;
	else
		token->expnd = true;
	free(token->content);
	token->content = new_content;
}


