#include "../minishell.h"

char    **to_array(t_env    *env)
{
    int i;
    char    **a_env;
    int idx;
    t_env   *tmp;
    char    *tmp2;

    i = 0;
    idx = 0;
    tmp = env;
    while (tmp)
    {
        i++;
        tmp = tmp->next;
    }
    tmp = env;
    a_env = malloc(sizeof(char *) * (i + 1));
    if (!a_env)
       return (NULL);
    while (i > idx)
    {
       tmp2 = ft_strjoin(tmp->key, "=");
       if(!tmp2)
            return (NULL);
       a_env[idx] = ft_strjoin(tmp2, tmp->value);
       free(tmp2);
       idx++;
       tmp = tmp->next;
    }
    a_env[idx] = NULL;
    return (a_env);
}

char    **get_path(char  *cmd, t_env  *env)
{
    t_env   *node;
    char    *path;
    char    **all_paths;
    int     i;
    char    *tmp;

    i = 0;
    node = find_key(env, "PATH");
    if (!node)
        return (NULL);
    path = node->value;
    all_paths = ft_split(path, ':');
    if (!all_paths)
        return (NULL);
    tmp = ft_strjoin("/", cmd);
    if (!tmp)
        return (NULL);
    cmd = tmp;
    while(all_paths[i])
    {
        tmp = ft_strjoin(all_paths[i], cmd);
        if (!tmp)
            return (NULL);
        free(all_paths[i]);
        all_paths[i] = tmp;
        i++;
    }
    all_paths[i] = NULL;
    free(cmd);
    return (all_paths);
}

char    *true_path(char  *cmd, t_env *env)
{
    char    **all_paths;
    int     i;
    char    *path;

    path = NULL;
    i = 0;
    if (!cmd || !cmd[0])
        return (NULL);
    if (cmd && ft_strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return (ft_strdup(cmd));
        else
            return (NULL);
    }
    all_paths = get_path(cmd, env);
    if (!all_paths)
        return (NULL);
    while (all_paths[i])
    {
        if (access(all_paths[i], X_OK) == 0)
        {
            path = ft_strdup(all_paths[i]);
            break;
        }
        i++;
    }
    free_arr(all_paths);
    return (path);
}
