#include "../minishell.h"

void    free_arr(char **arr)
{
    int i;

    i = 0;
    while (arr && arr[i])
        free(arr[i++]);
    free(arr);
}