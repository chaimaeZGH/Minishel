/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solo_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rroundi <rroundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 12:40:48 by rroundi           #+#    #+#             */
/*   Updated: 2025/08/04 20:50:09 by rroundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	path_err(char **e_arr, t_env *env, char **cmd)
{
	free_arr(e_arr);
	printf("%s :command not found\n", cmd[0]);
	env->exit_s = 127;
	return (127);
}

int	fork_err(char *path, char **e_arr)
{
	perror("fork failed");
	free_arr(e_arr);
	free(path);
	return (1);
}

void	for_exec(t_env *env, char *path, char **cmd, char **e_arr)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	execve(path, cmd, e_arr); 
	env->exit_s = handle_exec(cmd);
	free_arr(e_arr);
	free(path);
	exit(env->exit_s);
}

int	for_child_exit(int status)
{
	int	ret;

	ret = 0;
	if (WIFEXITED(status))
		ret = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
	{
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
	return (ret);
}

int	exec_bin(char **cmd, t_env *env)
{
	pid_t			pid;
	char			**e_arr;
	struct termios	term;
	char			*path;
	int				status;

	e_arr = to_array(env);
	if (!e_arr)
		return (1);
	path = true_path(cmd[0], env);
	if (!path)
		return (path_err(e_arr, env, cmd));
	tcgetattr(STDIN_FILENO, &term);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
		return (fork_err(path, e_arr));
	if (pid == 0)
		for_exec(env, path, cmd, e_arr);
	waitpid(pid, &status, 0);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	env->exit_s = for_child_exit(status);
	setup_signals();
	free_arr(e_arr);
	return (free(path), env->exit_s);
}
