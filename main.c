
#include "minishell.h"

int g_sig = 0;

void	free_node(t_tokenlist *node)
{
	free(node->content);
	free(node);
}

void	add_new_conten(t_tokenlist *current, t_tokenlist *prevv, char *new_c)
{
	size_t	new_len;

	new_len = ft_strlen(current->content) + ft_strlen(prevv->content) + 1;
	new_c = malloc(new_len);
	if (!new_c)
		return ;
	ft_strlcpy(new_c, prevv->content, new_len);
	ft_strlcat(new_c, current->content, new_len);
	free(prevv->content);
	prevv->content = new_c;
	prevv->next = current->next;
	if (current->next)
		current->next->prev = prevv;
	free_node(current);
}

char	*map_type(t_type_list type)
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

void	print_nodes(t_tokenlist *head)
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
		if ((current->type != PIPE && current->type !=comnd) && 
			(current->next->type!=comnd && current->next->type!=PIPE))
			return (ft_error_msg(1), ++j);
		current = current->next;
	}
	return (j);
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

void	continue_merge(t_tokenlist **head)
{
	t_tokenlist	*current;
	t_tokenlist	*next_node;

	current = *head;
	while (current)
	{
		if (chaeck_content(current) == 1)
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

void	merge_file_cmd(t_tokenlist **head)
{
	if (!head || !*head)
		return ;
	continue_merge(head);
}

void	three_lines(t_check *strackt, int i, int j, char *cmd)
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

void    skip_whitespaces(int *i, char *line)
{
	while (line[*i] == ' ' || line[*i] == '\t')
		(*i)++;
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
		add_new_conten(current, prevvv, new_content);
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
	else if (line[*i] == '<' && line[(*i)+1] == '<')
		{
		ft_lstadd_backn(head, ft_lstnewn(ft_strdup("<<")));
		(*i) += 2;
	}
	else if (line[*i] == '<') 
	{
		ft_lstadd_backn(head, ft_lstnewn(ft_strdup("<")));
		(*i)++;
	}
	else if (line[*i] == '>' && line[(*i)+1] == '>') 
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


int	handle_regular_cmd(char *line, int *i, t_tokenlist **head)
{
	t_check	*result;

	result = extract_cmd(line, *i);
	if (!result)
		return (printf(RED"Memory allocation error\n"RESET), 1);
	ft_lstadd_backn(head, result->str);
	*i = result->i;
	free(result);
	return (0);
}

int	handle_quotes(char *line, int *i, t_tokenlist **head, char **ll)
{
	t_check	*result;
	int		j;

	j = *i;
	result = extract_cmd_quat(line, *i, line[*i]);
	if (!result)
		return (printf(RED"Syntax error: unclosed quotes\n"RESET), 1);
	ft_lstadd_backn(head, result->str);
	*i = result->i;
	free(result);
	if (j > 0 && line[j - 1] != ' ')
		merg_last_with_one(head);
	if (line[*i] != ' ' && line[*i] != '\0')
		*ll = (ft_lstlastn(*head))->content;
	return (0);
}

int	process_line_part(char *line, int *i, t_tokenlist **head, char **ll)
{
	skip_whitespaces(i, line);
	if (line[*i] == '\0')
		return (0);
	if (line[*i] == '\'' || line[*i] == '\"')
		return (handle_quotes(line, i, head, ll));
	else if (line[*i] == '|' || line[*i] == '<' || line[*i] == '>')
	{
		handle_operators(i, line, head);
		return (0);
	}
	return (handle_regular_cmd(line, i, head));
}

int	split_line(char *line, t_tokenlist **head)
{
	int		i;
	char	*ll;

	i = 0;
	ll = NULL;
	while (line[i] != '\0')
	{
		if (process_line_part(line, &i, head, &ll) != 0)
			return (1);
		else
		{
			if (ll != NULL)
				merge_this_with_next(head, ll);
		}
	}
	return (0);
}

void	change_quots(t_combo *new, int j, char quote_char)
{
	while (new->str[new->i] != '\0')
	{
		if ((new->str[new->i] == '\'' || new->str[new->i] == '\"') && j < 2)
		{
			if (j == 0)
			{
				quote_char = new->str[new->i];
				if (quote_char == '\'')
					new->str[new->i] = 1;
				else
					new->str[new->i] = 2;
				j = 1;
			}
			else if (new->str[new->i] == quote_char)
			{
				if (quote_char == '\'')
					new->str[new->i] = 1;
				else
					new->str[new->i] = 2;
				j = 0;
			}
		}
		(new->i)++;
	}
}

void	process_token_content(t_tokenlist *token)
{
	int		j;
	char	quote_char;
	t_combo	*new_cont;
	char	*tmp;

	if (!token || !token->content)
		return ;
	new_cont = malloc(sizeof(t_combo));
	if (!new_cont)
		return ;
	new_cont->str = strdup(token->content);
	if (!new_cont->str)
		return ;
	new_cont->i = 0;
	j = 0;
	quote_char = 0;
	change_quots(new_cont, j, quote_char);
	tmp = new_cont->str;
	free(token->content);
	token->content = tmp;
	free(new_cont);
}

void	replace_eache_node(t_tree_list *root)
{
	t_tokenlist	*current_arg;
	t_tokenlist	*current_in;
	t_tokenlist	*current_out;

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
}

void	replace_quotes(t_tree_list *root)
{
	if (!root)
		return ;
	replace_eache_node(root);
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

int	initial_new_list(t_tokenlist *token, char **env, t_tokenlist **list, int exit_s)
{
	t_tokenlist	*new;
	char		*new_content;

	while (token)
	{
		new_content = expand_content(token->content, env, exit_s);
		if ((ft_strlen(token->content) != 0 && ft_strlen(new_content) == 0))
			free(new_content);
		else
		{
			new = malloc (sizeof(t_tokenlist));
			if (!new)
				return (0);
			new->content = new_content;
			new->next = NULL;
			new->prev = NULL;
			if(ft_strchr(token->content,'$') != NULL && ft_strchr(new_content,' ')!=NULL)
				new->expnd = false;
			else
				new->expnd = true;
			ft_lstadd_backn(list, new);
		}
		token = token->next;
	}
	return (1);
}

t_tokenlist	*handle_args(t_tokenlist *token, char **env, int exit_s )
{
	t_tokenlist	*list;
	t_tokenlist	*tmp;
	int			a;

	list = NULL;
	tmp = token;
	a = initial_new_list(token, env, &list, exit_s);
	if (a == 0)
		return (NULL);
	free_list(tmp);
	return (list);
}

void	check_expend_heardoc(t_tokenlist *current_in, char **env, int exit_s)
{
	t_tokenlist	*next;

	while (current_in)
	{
		if (current_in->type == HEREdocument)
		{
			if (check_quotes(current_in->content) == 1)
				current_in->expnd = false;
		}
		next = current_in->next;
		if (current_in->type != HEREdocument)
			process_expend_content(current_in, env, exit_s);
		current_in = next;
	}
}

void	expend_check(t_tree_list *root, char **env, int exit_s)
{
	t_tokenlist	*current_in;
	t_tokenlist	*current_out;
	t_tokenlist	*next;

	if (root->type == comnd && root->cmd)
	{
		current_in = root->cmd->in;
		current_out = root->cmd->out;
		root->cmd->args = handle_args(root->cmd->args, env, exit_s);
		check_expend_heardoc(current_in, env, exit_s);
		while (current_out)
		{
			next = current_out->next;
			process_expend_content(current_out, env, exit_s);
			current_out = next;
		}
	}
}

void	check_for_expend(t_tree_list *root, char **env, int exit_s)
{
	if (!root)
		return ;
	expend_check(root, env, exit_s);
	check_for_expend(root->right, env, exit_s);
	check_for_expend(root->left, env, exit_s);
}

void	check_string(char *old, int *i, int *j)
{
	while (old[*i] != '\0')
	{
		if (old[*i] != 1 && old[*i] != 2)
			(*j)++;
		(*i)++;
	}
}

void	fill_string(char *old, char *new)
{
	int	i;
	int	j;

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
}

void	deleet_quots(t_tokenlist *str)
{
	char	*new;
	char	*old;
	int		i;
	int		j;

	j = 0;
	i = 0;
	old = str->content;
	check_string(old, &i, &j);
	new = malloc(j + 1);
	if (!new)
		return ;
	fill_string(old, new);
	free(str->content);
	str->content = new;
}

void	call_remove_quotes(t_tokenlist *current)
{
	while (current)
	{
		if (current->content)
			deleet_quots(current);
		current = current->next;
	}
}

void	check_remove_quots(t_tree_list *root)
{
	t_tokenlist	*current_arg;
	t_tokenlist	*current_in;
	t_tokenlist	*current_out;

	if (root->type == comnd && root->cmd)
	{
		if (root->cmd->args)
		{
			current_arg = root->cmd->args;
			call_remove_quotes(current_arg);
		}
		if (root->cmd->in)
		{
			current_in = root->cmd->in;
			call_remove_quotes(current_in);
		}
		if (root->cmd->out)
		{
			current_out = root->cmd->out;
			call_remove_quotes(current_out);
		}
	}
}

void	remove_quots(t_tree_list *root)
{
	if (!root)
		return ;
	check_remove_quots(root);
	remove_quots(root->right);
	remove_quots(root->left);
}

int	ft_count(char const *s, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] != '\0')
			j++;
		while (s[i] != c && s[i] != '\0')
			i++;
	}
	return (j);
}

int	count_args(t_tokenlist *args)
{
	int	count;

	count = 0;
	while (args)
	{
		if (args->expnd == false)
		{
			count= count + ft_count(args->content, ' ');
		}
		else
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

bool	copy_split(int *i, t_cmdlist *cmd, t_tokenlist *args)
{
	int		n;
	char	**spl;

	n = 0;
	spl = ft_split(args->content,' ');
	while(spl[n] != NULL)
	{
		cmd->cmd[*i] = strdup(spl[n]);
		if (!cmd->cmd[*i])
		{
			free_cmd_array(cmd->cmd, *i);
			cmd->cmd = NULL;
			return (false);
		}
		(*i)++;
		n++;
	}
	n = 0;
	while(spl[n] != NULL)
	{
		free(spl[n]);
		n++;
	}
	free(spl);
	return (true);
}

bool	copy_rest(int *i, t_cmdlist *cmd, t_tokenlist *args)
{
	cmd->cmd[*i] = strdup(args->content);
	if (!cmd->cmd[*i])
	{
		free_cmd_array(cmd->cmd, *i);
		cmd->cmd = NULL;
		return (false);
	}
	(*i)++;
	return (true);
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
		if(args->content != NULL)
		{
			if (args->expnd == false)
			{
				if (copy_split(&i, cmd, args) == false)
					return (false);
			}
			else
			{
				if (copy_rest(&i, cmd, args) == false)
					return (false);
			}
		}
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

void fill_double_point(t_tree_list *tree)
{
	if (!tree)
		return;
	if (tree->type == comnd)
		fill_cmd_from_args(tree->cmd);
	fill_double_point(tree->left);
	fill_double_point(tree->right);
}

t_tree_list   *init_shell(char *s, char **env, int	exit_s)
{
	t_tokenlist *head;
	t_tree_list *tree;

	head = NULL;
	tree = NULL;
	if(split_line(s,&head) == 0)
	{   
		if(syntax_erreur(head) == 0)
		{
			merge_file_cmd(&head);
			tree = create_tree(&head);
			replace_quotes(tree);//
			check_for_expend(tree, env, exit_s);
			remove_quots(tree);
			// check_empty_node(tree);
			fill_double_point(tree);
			// print_ast_topdown(tree);
			// print_nodes(head);//
		}
		else
			free_list(head);
	}
	else
		free_list(head);
	free(s);
	return (tree);
}

void handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_sig = SIGINT;
}

void setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	rl_catch_signals = 0;
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

void	cleanup(t_tree_list *tree, char	**a_env)
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
	t_tree_list	*tree;
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
		tree = init_shell(s, a_env, env->exit_s);
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
