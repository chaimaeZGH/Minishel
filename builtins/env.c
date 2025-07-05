#include "../minishell.h"

int ft_env(t_env    *env)
{
    t_env *curr;

    if (!env)
        return(1);
    curr = env;
    while (curr != NULL)
    {
        if (curr->value)
            printf("%s=%s\n", curr->key, curr->value);
        curr = curr->next;
    }
    return (0);
}

// int main (int ac, char **av, char **env)
// {
//     ft_env(env);
//     return (0);
// }
