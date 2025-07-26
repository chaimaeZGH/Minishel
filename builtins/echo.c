#include "../minishell.h"

int if_flag(char *av)
{
    int i;

    i = 0;
    if (!av[i] || av[i] != '-')
        return (0);
    i++;
    if (av[i] == '\0')
        return (0);
    while (av[i] && av[i] == 'n')
        i++;
    if (av[i] != '\0')
        return (0);
    return (1);
}

int ft_echo(char    **av)
{
    int i;
    int flag;

    flag = 1;
    i = 1;
    while (av[i] && if_flag(av[i]))
    {
        flag = 0;
        i++;
    }
    while (av[i])
    {
        printf("%s", av[i]);
        if (av[i + 1])
            printf(" ");
        i++;
    }
    // printf("%s\n", av[1]);
    if (flag == 1)
        printf("\n");
    return (0);
}

// int main ()
// {
//     char *av[6] = { "echo", "-nnnn", "-nnnnnn" ,"reda", NULL };
//     ft_echo(av);
// }