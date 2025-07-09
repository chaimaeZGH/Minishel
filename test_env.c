#include "./libft/libft.h"

#include <stdio.h>
#include <string.h>

char *get_env_value(char **env, const char *var)
{
    int i = 0;
    size_t len = ft_strlen(var);
    while (env[i] != NULL)
    {
        if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
            return env[i] + len + 1;
        i++;
    }
    return NULL;
}

int main(int argc, char **argv, char **env) 
{
    (void)argc;
    (void)argv;

    char input[256];
    printf("Enter variable name (with $):");
    if (!fgets(input, sizeof(input), stdin))
        return 1;

    // Remove newline if present
    input[strcspn(input, "\n")] = '\0';

    if (input[0] == '$')
    {
        char *value = get_env_value(env, input + 1);
        if (value)
            printf("Value of %s:%s\n", input, value);
        else
            printf("Variable %s not found.\n", input);
    }
    else
    {
        printf("Input must start with '$'\n");
    }

    return 0;
}

