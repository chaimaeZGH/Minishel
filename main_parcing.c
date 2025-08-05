/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parcing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/08/05 21:26:27 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	efacts(t_token *head, t_env *envp)
{
	envp->exit_s = 258;
	free_list(head);
}

t_tree	*init_shell(char *s, char **env, t_env *envp)
{
	t_token	*head;
	t_tree	*tree;

	head = NULL;
	tree = NULL;
	if (split_line(s, &head) == 0)
	{
		if (syntax_erreur(head) == 0)
		{
			merge_file_cmd(&head);
			tree = create_tree(&head);
			replace_quotes(tree);
			check_for_expend(tree, env, envp->exit_s);
			remove_quots(tree);
			fill_double_point(tree);
		}
		else
			efacts(head, envp);
	}
	else
		efacts(head, envp);
	return (free(s), tree);
}

void	for_signals(t_env *env, char *s, char **a_env)
{
	if (g_sig == SIGINT)
	{
		if (env)
			env->exit_s = 1;
		g_sig = 0;
	}
	if (!s)
	{
		printf("exit\n");
		if (a_env)
			free_arr(a_env);
		exit(0);
	}
	if (*s)
		add_history(s);
}
