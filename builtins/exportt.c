#include "../minishell.h"

int if_valid(char *av)
{
    int i;

    i = 0;
    if (ft_isalpha(av[i]) == 0 && av[i] != '_')
        return (0);
    i++;
    while (av[i] && av[i] != '=')
    {
        if (ft_isalnum(av[i]) == 0 && av[i] != '_')
            return (0);
        i++;
    }
    return (1);
}


char    **get_values(char   *av)
{
    char    **div;
    char    *a_equa;

    div = malloc(sizeof(char *) * 3);
    a_equa = ft_strchr(av, '=');
    div[0] = ft_substr(av, 0, sublen(av));
    div[1] = ft_substr(a_equa, 1, ft_strlen(a_equa));
    div[2] = NULL;
    return (div);
}

int env_idx(char **env)
{
    int i;

    i = 0;
    while(env[i])
        i++;
    return (i);
}

char **env_dup(char **env)
{
    int i;
    int idx;
    char    **e_copy;

    i = 0;
    idx = env_idx(env);
    e_copy = malloc(sizeof(char *) * (idx + 1));
    while (env[i])
    {
        e_copy[i] =  ft_strdup(env[i]);
        i++;
    }
    e_copy[i] = NULL;
    return (e_copy);
}

void    set_env_variables(char  **env, char *var, char *value)
{
    char **e_copy;

    e_copy = env_dup(env);
}

int ft_export(char **av, char **env)
{
    int i;
    int ret;
    char **div;

    i = 1;
    ret = 0;
    if (!av[i])
        return (ft_env(env));
    while (av[i])
    {
        if (!if_valid(av[i]))
        {
            error_msg("export", "is not a valid indentifier", av[i]);
            ret = 1;
        }
        else if (ft_strchr(av[i], '=') != NULL)
        {
            div = get_values(av[i]);
            set_env_variables(env, div[0], div[1]);
            //add a function that frees the double pointer;
            // so you need a struct to be able to modify on the env
        }
        i++;
    }
}