#include "../minishell.h"

int print_error(char *cmd, char *msg)
{
    ft_putstr_fd("minishell :", 2);
    ft_putstr_fd(cmd, 2);
    ft_putendl_fd(msg , 2);
    return (1);
}

int error_msg(char *cmd, char *msg, char *av)
{
    ft_putstr_fd("minishell :", 2);
    ft_putstr_fd(cmd, 2);
    ft_putstr_fd(av, 2);
    ft_putendl_fd(msg , 2);
    return (1);
}

int sublen(char *av)
{
    int i;

    i = 0;
    while (av[i] && av[i] != '=')
        i++;
    return (i);
}