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
    free(nbr);
    return (str);
}

void    if_succes(char  *line, char *file_name, int  fd)
{
    free(line);
    free(file_name);
    close(fd);
    exit(0);
}

void    no_fd(char  *file_name)
{
    perror("failed to open heredoc");
    free(file_name);
    exit(1);
}
void    write_in_file(t_tokenlist *curr, char   *file_name, char **env)
{
    int fd;
    char    *line;
    char    *expanded_line;

    fd = open (file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1)
        no_fd(file_name);
    while (1)
    {
        signal(SIGINT, handle_sighere);
        line = readline("> ");
        if (!line || ft_strcmp(line, curr->content) == 0)
            break;
        if (curr->expnd == true)
        {
            expanded_line = expand_content(line, env);
            free(line);
            line = expanded_line;
        }
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    if_succes(line, file_name, fd);
}


int heredoc_redir(t_tokenlist  *curr, char  **env)
{
    static int  file;
    char    *file_name;
    pid_t   pid;
    int     status;
    struct termios term;

    curr->fd = file;
    file_name = generate_filename(file++);
    tcgetattr(STDIN_FILENO, &term);
    signal(SIGINT, SIG_IGN);
    pid = fork();
    if (pid == 0)
        write_in_file(curr, file_name, env);
    waitpid(pid, &status, 0);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
    {
        signal(SIGINT, handle_sigint);
        return (free(file_name), -1);
    }
    free(file_name);
    return (0);
}

int for_inredir(t_tokenlist *curr, t_tokenlist *in)
{
    int fd;

    fd =  open (in->content, O_RDONLY);
    if (fd == -1)
    {
        perror("open in");
        return (-1);
    }
    if (curr->next == NULL)
        dup2(fd, 0);
    close(fd);
    return (0);
}

int for_heredoc(t_tokenlist *curr)
{
    int     fd;
    char    *file_name;

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
    return (0);
}
int in_redir(t_tokenlist    *in, int    *s_stdin)
{
    t_tokenlist *curr;

    *s_stdin = dup(0);
    if (*s_stdin == -1)
        return (-1);
    curr = in;
    while (curr)
    {
        if (curr->type == INredirection)
        {
            if (for_inredir(curr, in) == -1)
                return (-1);
        }
        if (curr->type == HEREdocument)
        {
            if (for_heredoc(curr) == -1)
                return (-1);
        }
        curr = curr->next;
    }
    return (0);
}

int in_redir_call(t_tree_list   *tree, int  *s_stdin, t_env   **env)
{
    int ret;

    if (tree->cmd->in)
    {
        if(tree->cmd->in->type == INredirection || tree->cmd->in->type == HEREdocument)
        {
            ret = in_redir(tree->cmd->in, s_stdin);
            if (ret == -1)
                return ((*env)->exit_s = 1, ret);
        }
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

int out_redir_call(t_tree_list *tree, int   *s_stdout, t_env   **env)
{
    int ret;

   if (tree->cmd->out && (tree->cmd->out->type == OUTredirection || tree->cmd->out->type == OUTappend))
    {
        ret = out_redir(tree->cmd->out, s_stdout);
        if (ret == -1)
            return ((*env)->exit_s = 1, ret);
    }
    return (0);
}

int no_command(int  s_stdin, int   s_stdout)
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

void    restore(int s_stdin, int s_stdout)
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
}

int execute_with_redirections(t_tree_list *tree, t_env **env)
{
    int s_stdin = -1;
    int s_stdout = -1;
    int ret;

    ret = 0;
    ret = in_redir_call(tree, &s_stdin, env);
    if (ret == -1)
        return (ret);
    ret = out_redir_call(tree, &s_stdout, env);
    if (ret == -1)
        return (ret);
    if (!tree->cmd->cmd)
        return (no_command(s_stdin, s_stdout));
    ret = is_builtin(tree);
    if (ret != -1)
        ret = execute_builtins(tree, env, ret);
    else
        ret = exec_bin(tree->cmd->cmd, *env);
    (*env)->exit_s = ret;
    restore(s_stdin, s_stdout);
    return (ret);
}
