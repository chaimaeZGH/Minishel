
#include "minishell.h"

char *get_env_value(char **env, const char *name) 
{
	int i;
	size_t len;

	i = 0;
	len = ft_strlen(name);
	while (env && env[i]) 
    {
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return env[i] + len + 1;
		i++;
	}
	return NULL;
}

char *ft_strndup(const char *s, size_t n) 
{
    size_t len;
    char *copy;

    len = 0;
    while (len < n && s[len] != '\0')
        len++;
    copy = malloc(len + 1);
    if (!copy)
        return NULL;
    ft_memcpy(copy, s, len);
    copy[len] = '\0';
    return copy;
}

char *expand_variable(const char *str, int *index, char **env) 
{
	int start;
	int i;
	char *varname;
	char *value;

	start = *index + 1;
	i = start;
	if(str[start] == '0')
	{
		(*index)++;
		return (ft_strdup("minishell"));
	}
	if(ft_isdigit(str[start]) == 1)
	{
		(*index)++;
		return ft_strdup("");
	}
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	if (i == start)
		return ft_strdup("$");
	varname = ft_strndup(str + start, i - start);
	value = get_env_value(env, varname);
	free(varname);
	*index = i - 1;
	if (value)
		return ft_strdup(value);
	else
		return ft_strdup("");
}

char *expand_content(const char *content, char **env)
{
	char *result;
	size_t bufsize;
	char quote_char;
	int i;
	char c;
	char *expanded;
	size_t len_result;
	size_t len_exp;
	size_t len;
	char *tmp;

    result = malloc(1);
    if (!result)
        return NULL;
    result[0] = '\0';
    bufsize = 1;
    quote_char = 0;
    i = 0;
    while (content[i]) 
    {
        c = content[i];
        if ((c == 1 || c == 2)) 
        {
            if (quote_char == 0)
                quote_char = c;
            else if (quote_char == c)
                quote_char = 0;
            i++;
            continue;
        }
        if (c == '$' && quote_char != 1) 
        {
            expanded = expand_variable(content, &i, env);
            len_result = ft_strlen(result);
            len_exp = ft_strlen(expanded);
            tmp = malloc(len_result + len_exp + 1);
            if (!tmp)
            {
                free(result);
                free(expanded);
                return NULL;
            }
            ft_strlcpy(tmp, result, len_result + len_exp + 1);
            ft_strlcat(tmp, expanded, len_result + len_exp + 1);
            free(result);
            free(expanded);
            result = tmp;
            bufsize = len_result + len_exp + 1;
        } 
        else 
        {
            len = ft_strlen(result);
            if (len + 2 > bufsize)
            {
                bufsize = len + 2;
                result = realloc(result, bufsize);
                if (!result)
                    return NULL;
            }
            result[len] = c;
            result[len + 1] = '\0';
        }
        i++;
    }
    return result;
}

void process_expend_content(t_tokenlist *token, char **env) 
{
	if (!token || !token->content)
		return;
	char *new_content = expand_content(token->content, env);
	free(token->content);
	token->content = new_content;
}


