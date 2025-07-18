#include "../minishell.h"

char    *generate_filename(int  file)
{
    char     *nbr;
    char    *str;

    nbr = ft_itoa(file);
    str = ft_strjoin("/tmp/heredoc_",nbr);
    return (str);
}

int heredoc_redir(t_tokenlist  *curr)
{
    static int  file;
    char    *file_name;
    pid_t   pid;
    char    *line;
    int     fd;
    int     status;

    // if (curr->heredoc_prepared == 1)
        // return (0);/
    curr->fd = file;
    file_name = generate_filename(file++);
    pid = fork();
    if (pid == 0)
    {
        fd = open (file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (fd == -1)
        {
            perror("failed heredoc open");
            exit(1);
        }
        while (1)
        {
            line = readline("> ");
            if (!line || ft_strcmp(line, curr->content) == 0)
                break;
            write(fd, line, ft_strlen(line));
            write(fd, "\n", 1);
            free(line);
        }
        free(line);
        close(fd);
        free(file_name);
        exit(0);
    }
    else
    {
        waitpid(pid, &status, 0);
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
            return (-1);
        // if (curr->next == NULL)
        // {
        //     file_name = generate_filename(file -1);
        //     fd = open (file_name, O_RDONLY);
        //     if (fd == -1)
        //     {
        //         perror("failed heredoc openn");
        //         return (-1);
        //     }
        //     dup2(fd, 0);
        //     close(fd);
            free(file_name);
        // }
    }
    return (0);
}

int in_redir(t_tokenlist    *in, int    *s_stdin)
{
    t_tokenlist *curr;
    int fd;
    char    *file_name;

    *s_stdin = dup(0);
    if (*s_stdin == -1)
        return (-1);
    curr = in;
    while (curr)
    {
        if (curr->type == INredirection)
        {  
            fd =  open (in->content, O_RDONLY);
            if (fd == -1)
            {
                perror("open in");
                return (-1);
            }
            if (curr->next == NULL)
                dup2(fd, 0);
            close(fd);
        }
        if (curr->type == HEREdocument)
        {
            printf("%d\n\n", curr->fd);
            file_name = generate_filename(curr->fd);
            fd = open (file_name, O_RDONLY);
            free(file_name);
            if (fd == -1)
            {
                perror("heredoc");
                return (-1);
            }
            if (curr->next == NULL)
              {
                printf("YOU DUPPED THE FILE\n\n");
               dup2(fd, 0);
              }
                
            close(fd);
        }
        curr = curr->next;
    }
    return (0);
}

int out_redir(t_tokenlist *out, int *s_stdout)
{
    int fd;
    t_tokenlist *curr;

    curr = out;
    *s_stdout = dup(0);
    if (*s_stdout == -1)
        return (-1);
    while (curr)
    {
        if (curr->type == OUTredirection)
            fd = open (curr->content, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        else if (curr->type == OUTappend)
            fd = open (curr->content, O_CREAT | O_WRONLY | O_APPEND, 0644);
        if (fd == -1)
        {
            perror("open out");
            return (-1);
        }
        if (curr->next == NULL)
            dup2(fd, 1);
        close (fd);
        curr = curr->next;
    }
    return (0);
}

int execute_with_redirections(t_tree_list *tree, t_env **env)
{
    int s_stdin = -1;
    int s_stdout = -1;
    int ret;

    if (tree->cmd->in)
    {
        if(tree->cmd->in->type == INredirection || tree->cmd->in->type == HEREdocument)
        {
            ret = in_redir(tree->cmd->in, &s_stdin);
            if (ret == -1)
                return (ret);
        }
    }
    if (tree->cmd->out && (tree->cmd->out->type == OUTredirection || tree->cmd->out->type == OUTappend))
    {
        ret = out_redir(tree->cmd->out, &s_stdout);
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