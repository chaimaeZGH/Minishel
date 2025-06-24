#include <stdio.h>

int main(int argc, char **argv, char **env) {
    (void)argc;  // Silence unused warnings
    (void)argv;  // (Optional if you don't use them)

    printf("Environment variables:\n");
    for (char **ptr = env; *ptr != NULL; ptr++) {
        printf("%s\n", *ptr);
    }

    return 0;
}
