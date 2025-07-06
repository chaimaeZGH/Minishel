#include <stdio.h>

int main(int argc, char **argv, char **env) 
{
    (void)argc;
    (void)argv;
    char **ptr;

    ptr = env;
    int i = 0;
    printf("Environment variables:\n");
    while(ptr[i] != NULL)
    {
        printf("%s\n",ptr[i]);
        i++;
    }
    return 0;
}
