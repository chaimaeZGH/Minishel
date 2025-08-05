/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rroundi <rroundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 22:41:14 by rroundi           #+#    #+#             */
/*   Updated: 2025/08/04 22:51:39 by rroundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_env_list(t_env *head)
{
	t_env	*current;

	current = head;
	while (current != NULL)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}

void	ft_swap(char **key, char **s_key)
{
	char	*tmp;

	tmp = *key;
	*key = *s_key;
	*s_key = tmp;
}

t_env	*sort(t_env *env)
{
	t_env	*curr;
	t_env	*tmp;

	if (!env || !env->next)
		return (env);
	curr = env;
	while (curr->next != NULL)
	{
		tmp = curr->next;
		while (tmp != NULL)
		{
			if (strcmp(curr->key, tmp->key) > 0)
			{
				ft_swap(&curr->key, &tmp->key);
				ft_swap(&curr->value, &tmp->value);
			}
			tmp = tmp->next;
		}
		curr = curr->next;
	}
	return (env);
}

t_env	*copy_env_list(t_env *env)
{
	t_env	*new_list;
	t_env	*current;
	t_env	*new_node;

	new_list = NULL;
	current = env;
	while (current)
	{
		new_node = create_node(current->key, current->value);
		add_front(&new_list, new_node);
		current = current->next;
	}
	return (new_list);
}
