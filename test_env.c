#include "minishell.h"
// #include <stdio.h>
// #include <string.h>

// char *get_env_value(char **env, const char *var)
// {
//     int i = 0;
//     size_t len = ft_strlen(var);
//     while (env[i] != NULL)
//     {
//         if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
//             return env[i] + len + 1;
//         i++;
//     }
//     return NULL;
// }

// int main(int argc, char **argv, char **env) 
// {
//     (void)argc;
//     (void)argv;

//     char input[256];
//     printf("Enter variable name (with $):");
//     if (!fgets(input, sizeof(input), stdin))
//         return 1;

//     // Remove newline if present
//     input[strcspn(input, "\n")] = '\0';

//     if (input[0] == '$')
//     {
//         char *value = get_env_value(env, input + 1);
//         if (value)
//             printf("Value of %s:%s\n", input, value);
//         else
//             printf("Variable %s not found.\n", input);
//     }
//     else
//     {
//         printf("Input must start with '$'\n");
//     }

//     return 0;
// }


char *get_env_value(char **env, const char *name) 
{
	int i = 0;
	size_t len = ft_strlen(name);
	while (env && env[i]) 
    {
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return env[i] + len + 1;
		i++;
	}
	return NULL;
}

char *my_strndup(const char *s, size_t n) 
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
	int start = *index + 1;
	int i = start;

	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;

	if (i == start)
		return ft_strdup("$");

	char *varname = my_strndup(str + start, i - start);
	char *value = get_env_value(env, varname);
	free(varname);

	*index = i - 1;

	if (value)
		return ft_strdup(value);
	else
		return ft_strdup("");
}

char *expand_content(const char *content, char **env)
{
	char *result = calloc(1, 1);
	size_t bufsize = 1;
	char quote_char = 0;

	for (int i = 0; content[i]; i++) 
    {
		char c = content[i];
		if ((c == 1 || c == 2)) 
        {
			if (quote_char == 0)
				quote_char = c;
			else if (quote_char == c)
				quote_char = 0;
			continue;
		}
		if (c == '$' && quote_char != 1) 
        {
			char *expanded = expand_variable(content, &i, env);
			size_t len_result = ft_strlen(result);
			size_t len_exp = ft_strlen(expanded);
			char *tmp = malloc(len_result + len_exp + 1);
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
			size_t len = ft_strlen(result);
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

