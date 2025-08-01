#include "../minishell.h"

int args_count(char **av)
{
    int i;

    i = 0;
    while (av[i])
        i++;
    return (i);
}

void    update_pwd(t_env **c_env)
{
    char cwd[PATH_MAX];
    t_env *old_pwd;

    old_pwd = find_key(*c_env, "PWD");
    if (old_pwd && old_pwd->value)
        set_env_value(c_env, "OLDPWD", old_pwd->value);
    if (getcwd(cwd, PATH_MAX) == NULL)
    {
        print_error("cd", "error retreving current working directory");
        return ;
    }
    set_env_value(c_env, "PWD", cwd);
}

int ft_cd(t_env **c_env, char **av)
{
    int res;
    char *cwd;
    t_env *envv;

    if (args_count(av) == 1)
    {
        envv = find_key(*c_env, "HOME");
        if (!envv || !envv->value)
        {
            print_error("cd", "HOME NOT SET");
            return (1);
        }
        cwd = envv->value;
        if (chdir(cwd) != 0)//chdit(path);
            perror("cd");
    }
    else
    {
        res = chdir(av[1]);
        if (res == 0)
            update_pwd(c_env);
        else
            return (perror(av[1]), 1);
    }
    return (0);
}

// int main (int ac, char **av, char **env)
// {
//     t_env *c_env= copy_env(env);
//     char *ad[2] = {"cd", "../helper",NULL};
//     ft_cd(c_env, ad);
//     return (0);
// }