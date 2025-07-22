#include "../minishell.h"

int print_error(char *cmd, char *msg)
{
    ft_putstr_fd("minishell: ", 2);
    ft_putstr_fd(cmd, 2);
    ft_putstr_fd(": ", 2);
    ft_putendl_fd(msg , 2);
    return (1);
}

int error_msg(char *cmd, char *msg, char *av)
{
    ft_putstr_fd("minishell: ", 2);
    ft_putstr_fd(cmd, 2);
    ft_putstr_fd(": ", 2);
    ft_putstr_fd(av, 2);
    ft_putstr_fd(": ", 2);
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

int   handle_exec(char    **cmd)
{
    if (errno == ENOENT)
    {
        fprintf(stderr, "%s :command not found", cmd[0]);
        return (127);
    }
    else if (errno == EACCES)
    {
        fprintf(stderr, "%s :permission denied", cmd[0]);
        return (126);
    }
    else
    {
        fprintf(stderr, "execve failed");
        return (1);
    }
}