/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/07/28 03:16:50 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*map_type(t_type_list type)//deleet
{
	if (type == 0)
		return ("command");
	if (type == 1)
		return ("pipe");
	if (type == 2)
		return ("redirection in");
	if (type == 3)
		return ("redirection out");
	if (type == 4)
		return ("redirection heredoc");
	if (type == 5)
		return ("redirection append");
	return (NULL);
}

void	print_nodes(t_tokenlist *head)//deleet
{
	t_tokenlist	*current;

	current = head;
	while (current != NULL)
	{
		printf(BLUE"Content: [%s], "YELLOW"type: %s \n"RESET, 
			current->content, map_type(current->type));
		current = current->next;
	}
}

void	free_list(t_tokenlist *head)
{
	t_tokenlist	*tmp;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp->content);
		free(tmp);
	}
}

void	ft_error_msg(int errnum)
{
	if (errnum == 0)
		printf(RED"Syntax error: unexpected token ; or \\\n"RESET);
	if (errnum == 1)
		printf(RED"syntax error near unexpected token\n"RESET);
	if (errnum == 2)
		printf(RED"syntax error near '|'\n"RESET);
}

int	syntax_erreur(t_tokenlist *head)
{
	int			i;
	int			j;
	t_tokenlist	*current;

	i = 0;
	j = 0;
	current = head;
	while (current != NULL)
	{
		if (current->type != comnd && current->next == NULL)
			return (ft_error_msg(1), ++j);
		if (current->type == PIPE && 
			(current->prev == NULL || current->prev->type != comnd))
			return (ft_error_msg(2), ++j);
		current = current->next;
	}
	return (j);
}

void	free_node(t_tokenlist *node)
{
	free(node->content);
	free(node);
}

int	chaeck_content(t_tokenlist *node)
{
	if ((node->type == INredirection && 
			ft_strncmp(node->content, "<", 1) == 0) || 
		(node->type == OUTredirection && 
			ft_strncmp(node->content, ">", 1) == 0) || 
		(node->type == HEREdocument && 
			ft_strncmp(node->content, "<<", 2) == 0) || 
		(node->type == OUTappend && 
			ft_strncmp(node->content, ">>", 2) == 0))
		return (1);
	else
		return (0);
}

void	merge_file_cmd(t_tokenlist **head)
{
	t_tokenlist	*current;
	t_tokenlist	*next_node;

	if (!head || !*head)
		return ;
	current = *head;
	while (current)
	{
		if (chaeck_content(current)==1)
		{
			next_node = current->next;
			if (next_node) 
			{
				next_node->type = current->type;
				if (current->prev)
					current->prev->next = next_node;
				else
					*head = next_node;
				next_node->prev = current->prev;
				free_node(current);
				current = next_node;
				continue ;
			}
		}
		current = current->next;
	}
}

void	three_lines(t_check	*strackt, int i, int j, char *cmd)
{
	strackt->str = ft_lstnewn(ft_strdup(cmd));
	strackt->i = j + i;
	free(cmd);
}

t_check	*extract_cmd_quat(char *line, int i, char c)
{
	char	*cmd;
	t_check	*strackt;
	int		j;
	int		k;

	j = i + 1;
	k = 0;
	while (line[j] && line[j] != c)
		j++;
	if (line[j] == '\0')
		return (NULL);
	cmd = malloc(j - i + 2);
	if (!cmd)
		return (NULL);
	while (i <= j)
		cmd[k++] = line[i++];
	cmd[k] = '\0';
	strackt = malloc(sizeof(t_check));
	if (!strackt) 
	{
		free(cmd);
		return (NULL);
	}
	three_lines(strackt, 1, j, cmd);
	return (strackt);
}

t_check	*extract_cmd(char *line, int i)
{
	char	*cmd;
	t_check	*strackt;
	int		j;
	int		k;

	j = i;
	k = 0;
	while (line[j] && line[j] != ' ' && line[j] != '|' && 
		line[j] != '<' && line[j] != '>' && 
		line[j] != '\'' && line[j] != '\"')
		j++;
	cmd = malloc(j - i + 1);
	if (!cmd)
		return (NULL);
	while (i < j)
		cmd[k++] = line[i++];
	cmd[k] = '\0';
	strackt = malloc(sizeof(t_check));
	if (!strackt)
	{
		free(cmd);
		return (NULL);
	}
	three_lines(strackt, 0, j, cmd);
	return (strackt);
}

void	skip_whitespaces(int *i, char *line)
{
	while (line[*i] == ' ' || line[*i] == '\t')
		(*i)++;
}
void	add_new_conten(t_tokenlist *current,t_tokenlist *prevvv, char *new_content)
{
	size_t		new_len;
	
	new_len = ft_strlen(current->content) + ft_strlen(prevvv->content) + 1;
	new_content = malloc(new_len);
	if (!new_content)
		return ;
	ft_strlcpy(new_content, prevvv->content, new_len);
	ft_strlcat(new_content, current->content, new_len);
	free(prevvv->content);
	prevvv->content = new_content;
	prevvv->next = current->next;
	if (current->next)
		current->next->prev = prevvv;
	free_node(current);
}

void	merge_this_with_next(t_tokenlist **lst, char *ll)
{
	t_tokenlist	*current;
	t_tokenlist	*last;
	char		*new_content;

	if (!lst || !*lst || !ll)
		return ;
	current = *lst;
	while (current && current->content != ll)
		current = current->next;
	if (!current)
		return ;
	last = current->next;
	if (!last || current->type != 0 || last->type != 0)
		return ;
	new_content = NULL;
	add_new_conten(last, current, new_content);
	if (new_content == NULL)
		return ;
}


void	merg_last_with_one(t_tokenlist **lst)
{
	t_tokenlist	*current;
	t_tokenlist	*prevvv;

	char		*new_content;

	if (!lst || !*lst || !(*lst)->next)
		return ;
	current = *lst;
	while (current->next != NULL)
		current = current->next;
	prevvv = current->prev;
	if (!prevvv)
		return ;
	if (current->type == 0 && prevvv->type == 0)
	{
		new_content = NULL;
		add_new_conten(current,prevvv, new_content);
		if (new_content == NULL)
			return ;
	}
}

void	handle_operators(int *i, char *line, t_tokenlist **head)
{
	if (line[*i] == '|') 
	{
		ft_lstadd_backn(head, ft_lstnewn(ft_strdup("|")));
		(*i)++;
	}
	else if (line[*i] == '<' && line[(*i) + 1] == '<')
	{
		ft_lstadd_backn(head, ft_lstnewn(ft_strdup("<<")));
		(*i) += 2;
	}
	else if (line[*i] == '<') 
	{
		ft_lstadd_backn(head, ft_lstnewn(ft_strdup("<")));
		(*i)++;
	}
	else if (line[*i] == '>' && line[(*i) + 1] == '>') 
	{
		ft_lstadd_backn(head, ft_lstnewn(ft_strdup(">>")));
		(*i) += 2;
	}
	else if (line[*i] == '>') 
	{
		ft_lstadd_backn(head, ft_lstnewn(ft_strdup(">")));
		(*i)++;
	}
}

int	split_line(char *line, t_tokenlist **head)
{
	t_check	*result;
	int		i;
	int		j;
	char	*ll;

	ll = NULL;
	i = 0;
	while (line[i] != '\0')
	{
		skip_whitespaces(&i, line);
		if (line[i] == '\0')
			break ;
		if (line[i] == '\'' || line[i] == '\"')
		{
			j = i;
			result = extract_cmd_quat(line, i, line[i]);
			if (!result)
				return (printf(RED"Syntax error: unclosed quotes\n"RESET), 1);
			ft_lstadd_backn(head, result->str);
			i = result->i;
			free(result);
			if (j > 0 && line[j - 1] != ' ')
				merg_last_with_one(head);
			if (line[i] != ' ' && line[i] != '\0')
				ll = (ft_lstlastn(*head))->content;
		}
		else if (line[i] == '|' || line[i] == '<' || line[i] == '>')
			handle_operators(&i, line, head);
		else
		{
			result = extract_cmd(line, i);
			if (!result) 
				return (printf(RED"Memory allocation error\n"RESET), 1);
			ft_lstadd_backn(head, result->str);
			i = result->i;
			free(result);
		}
		if (ll != NULL)
			merge_this_with_next(head, ll);
	}
	return (0);
}

void	process_token_content(t_tokenlist *token)
{
	char	*new_content;
	int		i;
	int		j;
	char	quote_char;

	if (!token || !token->content)
		return ;
	new_content = strdup(token->content);
	if (!new_content)
		return ;
	i = 0;
	j = 0;
	quote_char = 0;
	while (new_content[i] != '\0')
	{
		if ((new_content[i] == '\'' || new_content[i] == '\"') && j < 2)
		{
			if (j == 0)
			{
				quote_char = new_content[i];
				if (quote_char == '\'')
					new_content[i] = 1;
				else
					new_content[i] = 2;
				j = 1;
			}
			else if (new_content[i] == quote_char)
			{
				if (quote_char == '\'')
					new_content[i] = 1;
				else
					new_content[i] = 2;
				j = 0;
			}
		}
		i++;
	}
	free(token->content);
	token->content = new_content;
}

void	replace_quotes(t_tree_list *root)
{
	t_tokenlist	*current_arg;
	t_tokenlist	*current_in;
	t_tokenlist	*current_out;

	if (!root) 
		return ;
	if (root->type == comnd && root->cmd)
	{
		current_arg = root->cmd->args;
		current_in = root->cmd->in;
		current_out = root->cmd->out;
		while (current_arg) 
		{
			process_token_content(current_arg);
			current_arg = current_arg->next;
		}
		while (current_in) 
		{
			process_token_content(current_in);
			current_in = current_in->next;
		}
		while (current_out) 
		{
			process_token_content(current_out);
			current_out = current_out->next;
		}
	}
	replace_quotes(root->right);
	replace_quotes(root->left);
}

int	check_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == 1 || str[i] == 2)
			return (1);
		i++;
	}
	return (0);
}

void	check_for_expend(t_tree_list *root, char **env)
{
	t_tokenlist	*current_arg;
	t_tokenlist	*current_in;
	t_tokenlist	*current_out;

	if (!root)
		return ;
	if (root->type == comnd && root->cmd)
	{
		current_arg = root->cmd->args;
		current_in = root->cmd->in;
		current_out = root->cmd->out;
		while (current_arg)
		{
			t_tokenlist *next = current_arg->next;
			process_expend_content(current_arg, env);
			current_arg = next;
		}
		while (current_in)
		{
			if (current_in->type == HEREdocument)
			{
				if (check_quotes(current_in->content) == 1)
					current_in->expnd = false;
			}
			t_tokenlist *next = current_in->next;
			if (current_in->type != HEREdocument)
				process_expend_content(current_in, env);
			current_in = next;
		}
		while (current_out)
		{
			t_tokenlist *next = current_out->next;
			process_expend_content(current_out, env);
			current_out = next;
		}
	}
	check_for_expend(root->right, env);
	check_for_expend(root->left, env);
}

void	deleet_quots(t_tokenlist *str)
{
	char	*new;
	char	*old;
	int		i;
	int		j;

	i = 0;
	j = 0;
	old = str->content;
	while (old[i] != '\0')
	{
		if (old[i] != 1 && old[i] != 2)
			j++;
		i++;
	}
	new = malloc(j + 1);
	if (!new) 
		return ;
	i = 0;
	j = 0;
	while (old[i] != '\0')
	{
		if (old[i] != 1 && old[i] != 2)
		{
			new[j] = old[i];
			j++;
		}
		i++;
	}
	new[j] = '\0';
	free(str->content);
	str->content = new;
}

void	remove_quots(t_tree_list *root)
{
	t_tokenlist	*current_arg;
	t_tokenlist	*current_in;
	t_tokenlist	*current_out;

	if (!root)
		return ;
	if (root->type == comnd && root->cmd)
	{
		if (root->cmd->args)
		{
			current_arg = root->cmd->args;
			while (current_arg)
			{
				if (current_arg->content)
					deleet_quots(current_arg);
				current_arg = current_arg->next;
			}
		}
		if (root->cmd->in)
		{
			current_in = root->cmd->in;
			while (current_in)
			{
				if (current_in->content)
					deleet_quots(current_in);
				current_in = current_in->next;
			}
		}
		if (root->cmd->out)
		{
			current_out = root->cmd->out;
			while (current_out)
			{
				if (current_out->content)
					deleet_quots(current_out);
				current_out = current_out->next;
			}
		}
	}
	remove_quots(root->right);
	remove_quots(root->left);
}

int	count_args(t_tokenlist *args)
{
	int	count;

	count = 0;
	while (args)
	{
		count++;
		args = args->next;
	}
	return (count);
}

void	free_cmd_array(char **cmd, int i)
{
	while (i > 0)
		free(cmd[--i]);
	free(cmd);
}

bool	copy_args_to_cmd(t_cmdlist *cmd, t_tokenlist *args, int count)
{
	int	i;

	i = 0;
	cmd->cmd = malloc(sizeof(char *) * (count + 1));
	if (!cmd->cmd)
		return (false);
	while (args)
	{
		cmd->cmd[i] = strdup(args->content);
		if (!cmd->cmd[i])
		{
			free_cmd_array(cmd->cmd, i);
			cmd->cmd = NULL;
			return (false);
		}
		i++;
		args = args->next;
	}
	cmd->cmd[i] = NULL;
	return (true);
}

void	fill_cmd_from_args(t_cmdlist *cmd)
{
	int	count;

	if (!cmd || !cmd->args)
	{
		cmd->cmd = NULL;
		return ;
	}
	count = count_args(cmd->args);
	if (!copy_args_to_cmd(cmd, cmd->args, count))
		cmd->cmd = NULL;
}

void	fill_double_point(t_tree_list *tree)
{
	if (!tree)
		return ;
	if (tree->type == comnd)
		fill_cmd_from_args(tree->cmd);
	fill_double_point(tree->left);
	fill_double_point(tree->right);
}

void	init_shell(char *s, char **env)
{
	t_tokenlist	*head;
	t_tree_list	*tree;

	head = NULL;
	tree = NULL;
	if (split_line(s, &head) == 0)
	{
		if (syntax_erreur(head) == 0)
		{
			merge_file_cmd(&head);
			tree = create_tree(&head);
			replace_quotes(tree);
			check_for_expend(tree, env);
			remove_quots(tree);
			fill_double_point(tree);
			print_ast_topdown(tree);
		}
		else
			free_list(head);
	}
	else
		free_list(head);
	free(s);
	free_tree(tree);
}

int	main(int argc, char **argv, char **envp)
{
	char	*s;

	(void)argv;
	if (argc != 1)
		return (printf(RED"invalid number of arguments\nUsage: ./minishell\n"RESET), 1);
	while (1)
	{
		s = readline(GREEN"minishell> "RESET);
		if (!s)
			break ;
		if (*s) 
			add_history(s);
		init_shell(s, envp);
	}
	return (0);
}
