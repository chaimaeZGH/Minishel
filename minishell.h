#ifndef MINISHELL_H
#define MINISHELL_H

#include "libft/libft.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>

typedef struct shell_data{
    char **env;
    
}shell_data;

int print_error(char *cmd, char *msg);
int error_msg(char *cmd, char *msg, char *av);
int sublen(char *av);

#endif