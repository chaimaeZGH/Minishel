/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rroundi <rroundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/08/05 22:33:28 by rroundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_sig = 0;

void	handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_sig = SIGINT;
}

void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	rl_catch_signals = 0;
}

void	cleanup(t_tree *tree, char	**a_env)
{
	if (tree)
		free_tree(tree);
	free_arr(a_env);
}

t_env	*init_env(char **envp)
{
	t_env	*env;

	if (!envp || !envp[0])
	{
		ft_putstr_fd("minishel: impossible to run without environment\n", 2);
		exit(1);
	}
	env = copy_env(envp);
	env->exit_s = 0;
	setup_signals();
	return (env);
}

int	main(int argc, char **argv, char **envp)
{
	char		*s;
	int			ret;
	t_tree		*tree;
	char		**a_env;
	t_env		*env;

	(void)argv;
	(void)argc;
	env = init_env(envp);
	while (1)
	{
		a_env = to_array(env);
		s = readline("minishell> ");
		for_signals(env, s, a_env);
		tree = init_shell(s, a_env, env);
		if (tree && prepare_heredoc(tree, a_env, env->exit_s) == -1)
		{
			cleanup(tree, a_env);
			env->exit_s = 1;
			continue ;
		}
		ret = execute(tree, &env);
		cleanup(tree, a_env);
	}
	return (free_env_list(env), ret);
}
