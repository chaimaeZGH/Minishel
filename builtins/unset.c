#include "../minishell.h"

void    remove_node(t_env **env, char *key)
{
    t_env   *to_remove;
    t_env   *prev;

    to_remove = find_key(*env, key);
    if (!to_remove)
        return ;
    if (to_remove == *env)
        *env = (*env)->next;
    else
    {
        prev = *env;
        while (prev->next != NULL && prev->next != to_remove)
            prev = prev->next;
        prev->next = to_remove->next;
    }
    free(to_remove->key);
    free(to_remove->value);
    free(to_remove);
}

int ft_unset(t_env  **env, char **av)
{
    int i;
    int ret;

    i = 1;
    ret = 0;
    // printf("\nn++%s++\n", av[i]);
    while (av[i])
    {
        // printf("\n\nrah dkhel\n\n");
        if (!if_valid(av[i]) || ft_strchr(av[i], '=') != NULL)
        {
            error_msg("unset", "not a valid identifier", av[i]);
            ret = 1;
        }
        else
            remove_node(env, av[i]);
        i++;
    }
    return (ret);
}