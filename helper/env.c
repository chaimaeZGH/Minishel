/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rroundi <rroundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 22:37:35 by rroundi           #+#    #+#             */
/*   Updated: 2025/08/04 22:53:47 by rroundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*create_node(char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (key)
		new_node->key = ft_strdup(key);
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
	new_node->next = NULL;
	return (new_node);
}

void	add_front(t_env **head, t_env *new_node)
{
	if (*head == NULL)
	{
		*head = new_node;
		return ;
	}
	new_node->next = *head;
	*head = new_node;
}

t_env	*copy_env(char **env)
{
	int		i;
	t_env	*head;
	char	*eq_pos;
	char	*value;
	t_env	*new_node;

	head = NULL;
	i = 0;
	while (env[i])
	{
		eq_pos = ft_strchr(env[i], '=');
		if (eq_pos && eq_pos != env[i])
		{
			*eq_pos = '\0';
			value = eq_pos + 1;
			new_node = create_node(env[i], value);
			add_front(&head, new_node);
			*eq_pos = '=';
		}
		i++;
	}
	return (head);
}

t_env	*find_key(t_env *head, char *key)
{
	while (head)
	{
		if (strcmp(head->key, key) == 0)
			return (head);
		head = head->next;
	}
	return (NULL);
}

void	set_env_value(t_env **head, char *key, char *value)
{
	t_env	*node;
	t_env	*new_node;

	new_node = NULL;
	node = find_key(*head, key);
	if (node)
	{
		if (value)
		{
			free(node->value);
			node->value = ft_strdup(value);
		}
	}
	else
	{
		new_node = create_node(key, value);
		add_front(head, new_node);
	}
}
