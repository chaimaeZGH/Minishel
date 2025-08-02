/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rroundi <rroundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 21:41:55 by rroundi           #+#    #+#             */
/*   Updated: 2025/07/31 21:47:23 by rroundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	left_child(int *fd, t_tree_list *tree, t_env **env)
{
	int	ret;

	signal(SIGINT, SIG_DFL);
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	ret = execute(tree->left, env);
	exit(ret);
}

void	right_child(int *fd, t_tree_list *tree, t_env **env)
{
	int	ret;

	signal(SIGINT, SIG_DFL);
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	ret = execute(tree->right, env);
	exit(ret);
}

int	child_status(int status2, t_env **env)
{
	int	ret;

	ret = 0;
	if (WIFEXITED(status2))
		ret = WEXITSTATUS(status2);
	if (WIFSIGNALED(status2))
	{
		if (WTERMSIG(status2) == SIGINT)
		{
			printf("\n");
			ret = 130;
		}
		if (WTERMSIG(status2) == SIGQUIT)
		{
			printf("Quit: 3\n");
			ret = 131;
		}
	}
	(*env)->exit_s = ret;
	return (ret);
}

int	fork_error(int *fd, pid_t pid1)
{
	perror("fork failed");
	close(fd[0]);
	close(fd[1]);
	if (pid1 > 0)
		kill(pid1, SIGTERM);
	return (1);
}

int	execute_pipe(t_tree_list *tree, t_env **env)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status1;
	int		status2; 

	if (pipe(fd))
		return (perror("pipe failed"), 1);
	signal(SIGINT, SIG_IGN);
	pid1 = fork();
	if (pid1 == -1)
		return (fork_error(fd, pid1));
	if (pid1 == 0)
		left_child(fd, tree, env);
	pid2 = fork();
	if (pid2 == -1)
		return (fork_error(fd, pid2));
	if (pid2 == 0)
		right_child(fd, tree, env);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	signal(SIGINT, handle_sigint);
	return (child_status(status2, env));
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