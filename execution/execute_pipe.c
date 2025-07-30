#include "../minishell.h"

int execute_pipe(t_tree_list *tree, t_env   **env)
{
    int fd[2];
    pid_t pid1;
    pid_t pid2;
    int   status1;
    int   status2; 
    int   ret;

    ret = 0;
    if (pipe(fd))
    {
        perror("pipe failed");
        return (1);
    }
    signal(SIGINT, SIG_IGN);
    pid1 = fork();
    if (pid1 == -1)
    {
        perror("fork failed");
        close(fd[0]);
        close(fd[1]);
        return (1);
    }
    if (pid1 == 0)
    {
        signal(SIGINT, SIG_DFL);
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close (fd[1]);
        ret = execute(tree->left, env);
        exit(ret);
    }
    pid2 = fork();
    if (pid2 == -1)
    {
        perror("fork failed");
        close(fd[0]);
        close(fd[1]);
        kill(pid1, SIGTERM);
        return (1);;
    }
    if (pid2 == 0)
    {
        signal(SIGINT, SIG_DFL);
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        ret = execute(tree->right, env);
        exit(ret);
    }
    close(fd[0]);
    close(fd[1]);
    waitpid(pid1, &status1, 0);
    waitpid(pid2, &status2, 0);
    signal(SIGINT, handle_sigint);
    if (WIFEXITED(status2))
        ret = WEXITSTATUS(status2);
    if (WIFSIGNALED(status2))
    {
        
        if (WTERMSIG(status2) == SIGINT)
        {
            printf("\n");
            ret = 130;
        }
        if (WTERMSIG(status2) == SIGINT)
        {
            printf("QUIT\n");
            ret = 131;
        }
    }
    (*env)->exit_s = ret;
    return (ret);
}

// int execute_pipe(t_tree_list *tree, t_env **env)
// {
//     int fd[2];
//     pid_t pid1;
//     pid_t pid2;

//     if (!tree)
//         return (-1);
//     pipe(fd);
//     pid1 = fork();
//     if (pid1 == 0)
//     {
//         close(fd[0]);
//         dup2(fd[1], STDOUT_FILENO);
//         close(fd[1]);
//         exec_nofork(tree->left->cmd->cmd, *env);
//         printf("FORK 1 FAILED\n");
//     }
//     pid2 = fork();
//     if (pid2 == 0)
//     {
//         close(fd[1]);
//         dup2(fd[0], STDIN_FILENO);
//         close(fd[0]);
//         if (tree->right->type == PIPE)
//             execute(tree->right , env);
//         else
//             exec_nofork(tree->right->cmd->cmd, *env);
//         printf("SECOND FORK FAILED\n");
//         exit(1);
//     }
//     close(fd[0]);
//     close(fd[1]);
//     wait(NULL);
//     wait(NULL);
//     return (0);
// }