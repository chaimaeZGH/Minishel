/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rroundi <rroundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:45:35 by rroundi           #+#    #+#             */
/*   Updated: 2025/08/04 20:09:43 by rroundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_env(t_env *env)
{
	t_env	*curr;

	if (!env)
		return (1);
	curr = env;
	while (curr != NULL)
	{
		if (curr->value)
			printf("%s=%s\n", curr->key, curr->value);
		curr = curr->next;
	}
	return (0);
}

int	ft_call_env(t_env *env, char **cmd)
{
	if (cmd[0] && !cmd[1])
		return (ft_env(env));
	else
		return (1);
}
