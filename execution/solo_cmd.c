/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solo_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rroundi <rroundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 12:40:48 by rroundi           #+#    #+#             */
/*   Updated: 2025/08/01 23:38:58 by rroundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int     exec_bin(char   **cmd, t_env    *env)
{
    int ret;
    char    **e_arr;
    char    *path;
    pid_t   pid;
    int     status;
    struct termios term;

    ret = 1;
    // if (!cmd || !cmd[0] || cmd[0][0] == '\0')
    // {
    //      return (0);
    // }
    // if (ft_strcmp())
        
    e_arr = to_array(env);
    if (!e_arr)
        return(ret);
    path = true_path(cmd[0], env);
    if (!path)
    {
        free_arr(e_arr);
        fprintf(stderr, "%s :command not found\n", cmd[0]);
        env->exit_s = 127;
        return (127);        
    }
    tcgetattr(STDIN_FILENO, &term);
    signal(SIGINT, SIG_IGN);
    pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        free_arr(e_arr);
        free(path);
        return (ret);
    }
    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        execve(path, cmd, e_arr); 
        env->exit_s = handle_exec(cmd);
        free_arr(e_arr);
        free(path);
        exit(env->exit_s);
    }
    waitpid(pid, &status, 0);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    if (WIFEXITED(status))
        ret = WEXITSTATUS(status);
    if (WIFSIGNALED(status))
    {
        // ret = WTERMSIG(status);
        // ret = ret + 128;
        if (WTERMSIG(status) == SIGINT)
        {
            printf("\n");
            ret = 130;
        }
        if (WTERMSIG(status) == SIGQUIT)
        {
            printf("QUIT\n");
            ret = 131;
        }
    }
    setup_signals();
    env->exit_s = ret;
    free_arr(e_arr);
    free(path);
    return (ret);
}