#include "../minishell.h"

int ft_env(char **env)
{
    int i;

    i = 0;
    if(!env)
        return (1);
    while (env[i])
    {
        ft_putendl_fd(env[i], 1);
        i++;
    }
    return (0);
}

int main (int ac, char **av, char **env)
{
    ft_env(env);
    return (0);
}