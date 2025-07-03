#include "minishell.h"

// int main (int ac, char **av, char **env)
// {
//     av = NULL;
//     ac = 0;
//     t_env *c_env= copy_env(env);
//     print_env_list(c_env);
//     char *ad[2] = {"cd", NULL};
//     char *cwd = NULL;
//     cwd = getcwd(cwd, PATH_MAX);
//     printf("%s\n",cwd);
//     ft_cd(&c_env, ad);
//     cwd = getcwd(cwd, PATH_MAX);
//     printf("%s\n",cwd);
//     print_env_list(c_env);
// }
int main (int ac, char **av, char **env)
{
    av = NULL;
    ac = 0;
    t_env *c_env= copy_env(env);
    // print_env_list(c_env);
    char *ad[3] = {"export", "ZZ=NUMBER1", NULL};
    char *az[3] = {"export", NULL};
    ft_export(ad, &c_env);
    printf("AAAAA\n");
    ft_export(az, &c_env);
}