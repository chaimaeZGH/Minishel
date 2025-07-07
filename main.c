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
// int main (int ac, char **av, char **env)
// {
//     av = NULL;
//     ac = 0;
//         print_env_list(c_env);
//     char *an[3] = {"export", "ZZ", NULL};
//     char *ad[3] = {"export", "ZZ=44", NULL};
//     char *az[2] = {"export", NULL};
//     printf("\n\n");
//     ft_export(ad, &c_env);
//     printf("\n\n");
//     ft_export(an, &c_env);
//     ft_export(az, &c_env);
//     printf("\n\n");
//     print_env_list(c_env);
// }

// int main (int ac, char **arg, char **env)
// {
//     (void)ac;
//     (void)arg;
//     t_env *c_env= copy_env(env);
//     print_env_list(c_env);
//     printf("\n\n");
//     char **av = to_array(c_env);
//     int i = 0;
//     while (av[i])
//     {
//         printf("%s\n", av[i]);
//         i++;
//     }
// }

// int main (int ac, char **av, char **env)
// {
//     (void)av;
//     (void)ac;
//     t_env *c_env= copy_env(env);
//     char **er =  get_path("ls",c_env);
//     int i = 0;
//     while (er[i])
//     {
//         printf("%s\n", er[i]);
//         i++;
//     }
// }

// int main (int ac, char **av, char **env)
// {
//     (void)av;
//     (void)ac;
//     t_env *c_env = copy_env(env);
//     char *pat = path("lsff", c_env);
//     printf("%s\n", pat);
// }

int main(int ac, char **av, char **envp)
{
     (void)av;
    (void)ac;
    char *cmd[] = {"cat","minishell.h" ,NULL};
    t_env *env = copy_env(envp); 
    int status = exec_bin(cmd, env);
    printf("Command exited with status: %d\n", status);
}
