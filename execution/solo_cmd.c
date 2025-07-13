/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solo_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rroundi <rroundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 12:40:48 by rroundi           #+#    #+#             */
/*   Updated: 2025/07/11 18:43:16 by rroundi          ###   ########.fr       */
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
        return (ret);
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
        //need to check all cases of execve like (command not found) or (command found but not executable) or (generic code) 
        perror("execve failed");
        free_arr(e_arr);
        free(path);
        exit(1);
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
    free_arr(e_arr);
    free(path);
    return (ret);
}