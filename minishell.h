#ifndef MINISHELL_H
#define MINISHELL_H

#include "libft/libft.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


typedef struct s_env{
    char *key;
    char *value;
    struct s_env *next;
}t_env;

int print_error(char *cmd, char *msg);
int error_msg(char *cmd, char *msg, char *av);
int sublen(char *av);
t_env   *find_key(t_env *head, char *key);
void    set_env_value(t_env **head, char *key, char *value);
t_env   *copy_env(char **env);
int ft_cd(t_env **c_env, char **av);
void print_env_list(t_env *head);
t_env   *sort(t_env *env);
int ft_export(char **av, t_env **c_env);
int if_valid(char *av);
int ft_unset(t_env  **env, char **av);
char    **to_array(t_env    *env);
char    **get_path(char  *cmd, t_env  *env);
void    free_arr(char **arr);
char    *path(char  *cmd, t_env *env);

#endif