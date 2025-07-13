#include "../minishell.h"

// int execute_out_redirection(t_tree_list *tree, char *fn, t_env **env)
// {
//     int fd;
//     int tmp2;
//     int ret;

//     fd = open (fn, O_CREAT | O_WRONLY | O_TRUNC , 0644);
//     // if (fd == -1)
//     // handle error
//     tmp2 = dup(1);
//     dup2(fd, STDOUT_FILENO);
//     close(fd);
//     ret = is_builtin(tree);
//     if (ret != -1)
//         ret = execute_builtins(tree, env, ret);
//     else
//     {
//         ret = exec_bin(tree->cmd->cmd, *env);
//     }
//     dup2(tmp2, STDOUT_FILENO);
//     // dup2(tmp1, STDIN_FILENO);
//     return(ret);
// }

// int execute_in_redirection(t_tree_list *tree, char *fn, t_env **env)
// {
//     int fd;
//     int tmp;
//     int ret;

//     fd = open (fn , O_RDONLY , 0644);
//     tmp = dup(0);
//     dup2(fd, STDIN_FILENO);
//     close(fd);
//      ret = is_builtin(tree);
//     if (ret != -1)
//         ret = execute_builtins(tree, env, ret);
//     else
//     {
//         ret = exec_bin(tree->cmd->cmd, *env);
//     }
//     dup2(tmp, STDIN_FILENO);
//     // dup2(tmp1, STDIN_FILENO);
//     return(ret);
// }

int in_redir(char *fn, int *s_stdin)
{
    int fd;

    *s_stdin = dup(0);
    if (*s_stdin == -1)
        return (-1);
    fd = open(fn , O_RDONLY);
    if (fd == -1)
    {
        perror("open in ");
        return (-1);
    }
    dup2(fd, 0);
    close(fd);
    return (0);
}

int out_redir(char *fn, int *s_stdout)
{
    int fd;

    *s_stdout = dup(1);
    if (*s_stdout == -1)
        return (-1);
    fd = open(fn , O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open out ");
        return (-1);
    }
    dup2(fd, 1);
    close(fd);
    return (0);
}

int execute_with_redirections(t_tree_list *tree, t_env **env)
{
    int s_stdin = -1;
    int s_stdout = -1;
    int ret;

    if (tree->cmd->in && tree->cmd->in->type == INredirection)
    {
       ret = in_redir(tree->cmd->in->content, &s_stdin);
       if (ret == -1)
        return (ret);
    }
    if (tree->cmd->out && tree->cmd->out->type == OUTredirection)
    {
        ret = out_redir(tree->cmd->out->content, &s_stdout);
        if (ret == -1)
            return (ret);
    }
    ret = is_builtin(tree);
    if (ret != -1)
        ret = execute_builtins(tree, env, ret);
    else
        ret = exec_bin(tree->cmd->cmd, *env);
    if (s_stdin != -1)
    {
        dup2(s_stdin, STDIN_FILENO);
        close(s_stdin);
    }
    if (s_stdout != -1)
    {
        dup2(s_stdout, STDOUT_FILENO);
        close(s_stdout);
    }
    return ret;
}