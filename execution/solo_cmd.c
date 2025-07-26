/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solo_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rroundi <rroundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 12:40:48 by rroundi           #+#    #+#             */
/*   Updated: 2025/07/24 13:56:43 by rroundi          ###   ########.fr       */
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

    ret = 1;
    e_arr = to_array(env);
    if (!e_arr)
        return(ret);
    path = true_path(cmd[0], env);
    if (!path)
    {
        fprintf(stderr, "%s :command not found\n", cmd[0]);
        env->exit_s = 127;
        return (127);        
    }
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
        execve(path, cmd, e_arr); 
        env->exit_s = handle_exec(cmd);
        free_arr(e_arr);
        free(path);
        exit(env->exit_s);
    }
    if (waitpid(pid, &status, 0) == -1)
    {
        perror("waitpid failed");
        free_arr(e_arr);
        free(path);
        return (ret);
    }
    if (WIFEXITED(status))
        ret = WEXITSTATUS(status);
    if (WIFSIGNALED(status))
    {
        ret = WTERMSIG(status);
        ret = ret + 128;
    }
    env->exit_s = ret;
    free_arr(e_arr);
    free(path);
    return (ret);
}