#include "../minishell.h"

int ft_pwd(void)
{
    char *cwd;

    cwd = getcwd(NULL, 0);
    if (!cwd)
    {
        perror("PWD ERROR :");
        return (1);
    }
    ft_putendl_fd(cwd, 1);
    free(cwd);
    return (0);
}
// int main ()
// {
//     ft_pwd();
// }