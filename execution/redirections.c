#include "../minishell.h"

void    handle_sighere(int  sig)
{
    (void)sig;
    printf("\n");
    exit(1);
}
char    *generate_filename(int  file)
{
    char     *nbr;
    char    *str;

    nbr = ft_itoa(file);
    str = ft_strjoin("/tmp/heredoc_",nbr);
    return (str);
}

int heredoc_redir(t_tokenlist  *curr, char  **env)
{
    static int  file;
    char    *file_name;
    pid_t   pid;
    char    *line;
    int     fd;
    int     status;
    struct termios term;
    
    // if (curr->heredoc_prepared == 1)
        // return (0);/
    fd = 0;
    curr->fd = file;
    file_name = generate_filename(file++);
    tcgetattr(STDIN_FILENO, &term);
    signal(SIGINT, SIG_IGN);
    pid = fork();
    if (pid == 0)
    {
        fd = open (file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (fd == -1)
        {
            perror("failed heredoc open");
            free(file_name);
            exit(1);
        }
        // printf("this is the delimiter!: %s\n", rr->content);
        while (1)
        {
            signal(SIGINT, handle_sighere);
            line = readline("> ");
            if (!line || ft_strcmp(line, curr->content) == 0)
                break;
            if (curr->expnd == true)
                line = expand_content(line, env);
            write(fd, line, ft_strlen(line));
            write(fd, "\n", 1);
            free(line);
        }
        free(line);
        close(fd);
        free(file_name);
        exit(0);
    }
    waitpid(pid, &status, 0);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
    {
        signal(SIGINT, handle_sigint);
        return (free(file_name), close(fd), -1);
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
            file_name = generate_filename(curr->fd);
            fd = open (file_name, O_RDONLY);
            unlink(file_name);
            if (fd == -1)
            {
                perror("heredoc");
                return (-1);
            }
            if (curr->next == NULL)
            {
                dup2(fd, 0);
                close(fd);
            }
            free(file_name);    
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
    *s_stdout = dup(1);
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
            printf("\n\n%s\n", curr->content);
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

    ret = 0;
    if (tree->cmd->in)
    {
        if(tree->cmd->in->type == INredirection || tree->cmd->in->type == HEREdocument)
        {
            ret = in_redir(tree->cmd->in, &s_stdin);
            if (ret == -1)
            {
                (*env)->exit_s = 1;
                return (ret);
            }
        }
    }
    if (tree->cmd->out && (tree->cmd->out->type == OUTredirection || tree->cmd->out->type == OUTappend))
    {
        ret = out_redir(tree->cmd->out, &s_stdout);
        if (ret == -1)
        {
            (*env)->exit_s = 1;
            return (ret);
        }
    }
    if (!tree->cmd->cmd)
    {
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
        return (0);
    }
    
     ret = is_builtin(tree);
    if (ret != -1)
        ret = execute_builtins(tree, env, ret);
    else
        ret = exec_bin(tree->cmd->cmd, *env);
    (*env)->exit_s = ret;
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
    return (ret);
}