
#include "minishell.h"

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
		// if (current->type == comnd)
		// {
		//     i = 0;
		// 	while (current->content[i] != '\0')
		// 	{
		// 		if (current->content[i] == ';' || current->content[i] == '\\')
		// 			return (ft_error_msg(0), ++j);
		// 		i++;
		// 	}
		// }
		if (current->type != comnd && 
			(current->next == NULL || current->next->type != comnd))
			return (ft_error_msg(1), ++j);
		if (current->type == PIPE && 
			(current->prev == NULL || current->prev->type != comnd) )
			return (ft_error_msg(2), ++j);
		current = current->next;
	}
	return (j);
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
		if ((current->type == INredirection && 
			ft_strncmp(current->content, "<",1) == 0) || 
			(current->type == OUTredirection && 
			ft_strncmp(current->content, ">",1) == 0) || 
			(current->type == HEREdocument && 
			ft_strncmp(current->content, "<<",2) == 0) || 
			(current->type == OUTappend && 
			ft_strncmp(current->content, ">>",2) == 0))
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
				free(current->content);
				free(current);
				current = next_node;
				continue ;
			}
		}
		current = current->next;
	}
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
		return NULL;
	while (i <= j)
		cmd[k++] = line[i++];
	cmd[k] = '\0';
	strackt = malloc(sizeof(t_check));
	if (!strackt) 
	{
		free(cmd);
		return (NULL);
	}
	strackt->str = ft_lstnewn(ft_strdup(cmd));
	strackt->i = j + 1;
	free(cmd);
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
			line[j] != '<' && line[j] != '>' && line[j] != '\'' && line[j] != '\"')
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
	strackt->str = ft_lstnewn(ft_strdup(cmd));
	strackt->i = j;
	free(cmd);
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
	size_t		new_len;
	char		*new_content;

	if (!lst || !*lst || !ll)
		return;
	current = *lst;
	while (current && current->content != ll)
		current = current->next;
	if (!current)
		return;
	last = current->next;
	if (!last || current->type != 0 || last->type != 0)
		return;
	new_len = ft_strlen(current->content) + ft_strlen(last->content) + 1;
	new_content = malloc(new_len);
	if (!new_content)
		return;
	ft_strlcpy(new_content, current->content, new_len);
	ft_strlcat(new_content, last->content, new_len);
	free(current->content);
	current->content = new_content;
	current->next = last->next;
	if (last->next)
		last->next->prev = current;
	free(last->content);
	free(last);
}


void	merg_last_with_one(t_tokenlist **lst)
{
	t_tokenlist	*current;
	t_tokenlist	*prevvv;
	size_t		new_len;
	char		*new_content;

	if (!lst || !*lst || !(*lst)->next)
		return;
	current = *lst;
	while (current->next != NULL)
		current = current->next;
	prevvv = current->prev;
	if (!prevvv)
		return;
	if (current->type==0 && prevvv->type==0)
	{
		new_len = ft_strlen(current->content) + ft_strlen(prevvv->content) + 1;
		new_content = malloc(new_len);
		if (!new_content)
			return;
		ft_strlcpy(new_content, prevvv->content, new_len);
		ft_strlcat(new_content, current->content, new_len);
		free(prevvv->content);
		prevvv->content = new_content;
		prevvv->next = current->next;
		if (current->next)
			current->next->prev = prevvv;
		free(current->content);
		free(current);
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

int split_line(char *line, t_tokenlist **head)
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
				return printf(RED"Syntax error: unclosed quotes\n"RESET), 1;
			ft_lstadd_backn(head, result->str);
			i = result->i;
			free(result);
			if(j > 0 && line[j-1] != ' ')
				merg_last_with_one(head);
			if(line[i] != ' ' && line[i] != '\0')
				ll = (ft_lstlastn(*head))->content;
		}
		else if (line[i] == '|' || line[i] == '<' || line[i] == '>')
			handle_operators(&i, line, head);
		else
		{
			result = extract_cmd(line, i);
			if (!result) 
				return printf(RED"Memory allocation error\n"RESET), 1;
			ft_lstadd_backn(head, result->str);
			i = result->i;
			free(result);
		}
		if(ll != NULL)
			merge_this_with_next(head,ll);
	}
	return 0;
}
void	process_token_content(t_tokenlist *token) 
{
	char	*new_content;
	int		i;
	int		j;
	char	quote_char;

	if (!token || !token->content)
		return;
	new_content = strdup(token->content);
	if (!new_content)
		return;
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

void replace_quotes(t_tree_list *root) 
{
	t_tokenlist *current_arg;
	t_tokenlist *current_in;
	t_tokenlist *current_out;

	if (!root) 
		return;
	if(root->type == comnd && root->cmd)
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

int check_quotes(char *str)
{
	int i;
	
	i = 0;
	while(str[i] != '\0')
	{
		if(str[i] == 1 || str[i] == 2)
			return (1);
		i++;
	}
	return(0);
}
void check_for_expend(t_tree_list *root, char **env)
{
	t_tokenlist *current_arg;
	t_tokenlist *current_in;
	t_tokenlist *current_out;

	if (!root) 
		return;
	if(root->type == comnd && root->cmd)
	{
		current_arg = root->cmd->args;
		current_in = root->cmd->in;
		current_out = root->cmd->out;
		while (current_arg) 
		{
			process_expend_content(current_arg, env);
			current_arg = current_arg->next;
		}
		while (current_in)   
		{
			if(current_in->type == HEREdocument)
			{
				if(check_quotes(current_in->content) == 1)
					current_in->expnd = false;
			}
			if(current_in->type != HEREdocument)
				process_expend_content(current_in, env);
			current_in = current_in->next;
		}
		while (current_out) 
		{
			process_expend_content(current_out, env);
			current_out = current_out->next;
		}
	}
	check_for_expend(root->right, env);
	check_for_expend(root->left, env);
}

void deleet_quots(t_tokenlist *str)
{
	char *new;
	char *old = str->content;
	int i=0;
	int j=0;
	while (old[i] != '\0')
	{
		if(old[i] != 1 && old[i] != 2)
			j++;
		i++;
	}
	
	new = malloc(j+1);
	if (!new) 
		return;
	i=0;
	j=0;
	while(old[i] != '\0')
	{
		if(old[i] != 1 && old[i] != 2)
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

void remove_quots(t_tree_list *root)
{
	t_tokenlist *current_arg;
	t_tokenlist *current_in;
	t_tokenlist *current_out;

	if (!root) 
		return;
		
	if(root->type == comnd && root->cmd)
	{
		if (root->cmd->args) {
			current_arg = root->cmd->args;
			while (current_arg) 
			{
				if (current_arg->content)
					deleet_quots(current_arg);
				current_arg = current_arg->next;
			}
		}
		
		if (root->cmd->in) {
			current_in = root->cmd->in;
			while (current_in) 
			{
				if (current_in->content)
					deleet_quots(current_in);
				current_in = current_in->next;
			}
		}
		
		if (root->cmd->out) {
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

void fill_cmd_from_args(t_cmdlist *cmd)
{
	if (!cmd || !cmd->args)
	{
		cmd->cmd = NULL;
		return;
	}
	int count = 0;
	t_tokenlist *current = cmd->args;
	while (current)
	{
		count++;
		current = current->next;
	}
	cmd->cmd = (char **)malloc(sizeof(char *)*(count + 1));
	if (!cmd->cmd)
		return;
	current = cmd->args;
	int i = 0;
	while (current)
	{
		cmd->cmd[i] = strdup(current->content);
		if (!cmd->cmd[i])
		{
			while (i > 0)
				free(cmd->cmd[--i]);
			free(cmd->cmd);
			cmd->cmd = NULL;
			return;
		}
		i++;
		current = current->next;
	}
	cmd->cmd[count] = NULL;
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
void check_empty_node(t_tree_list *tree)
{
	if (!tree)
		return;
	t_tree_list *brnch = tree;
	while (brnch->right != NULL)
		brnch = brnch->right;
	if (!brnch->cmd || !brnch->cmd->args)
		return;
	t_tokenlist *dell = brnch->cmd->args;
	while (dell->next != NULL)
		dell = dell->next;

	if (ft_strlen(dell->content) == 0)
	{
		if (dell->prev)
			dell->prev->next = NULL;
		else
			brnch->cmd->args = NULL;
		free(dell->content);
		free(dell);
	}
}

t_tree_list   *init_shell(char *s, char **env)
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
			check_for_expend(tree, env);
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
    rl_on_new_line();  // Tell readline we're on a new line
    rl_replace_line("", 0);  // Clear the current input
    rl_redisplay();  // Redisplay the prompt
	g_sig = 1;
}

void setup_signals(void)
{
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, SIG_IGN);
	rl_catch_signals = 0;
}

int main(int argc, char **argv, char **envp)
{
    
	char        *s;
	int			ret;
    t_tree_list *tree;
	tree = NULL;
	char	**a_env;
    t_env   *env;
    (void)envp;
    (void)argv;
	setup_signals();
    env = copy_env(envp);
    if (argc != 1)
        return (printf(RED"invalid number of arguments\nUsage: ./minishell\n"RESET), 1);
	env->exit_s = 0;
	while (1)
	{
		if (g_sig == 1)
		{
			env->exit_s = 1;
			g_sig = 0;
		}
		a_env = to_array(env);
		if (a_env == NULL)
			return (1);
		if (isatty(fileno(stdin)))
			{
				s = readline(GREEN"minishell> "RESET);
				if (!s)
				{
					printf("exit\n");
					exit(0);
				}
			}
		else
		{
    		char buffer[1024];
    		int bytes_read = read(fileno(stdin), buffer, sizeof(buffer) - 1);
   			if (bytes_read <= 0)
      	 		 break;
    		buffer[bytes_read] = '\0';
    		s = ft_strtrim(buffer, "\n");
	}
		if (!s)
			break;
		if (*s) 
		    add_history(s);
		// printf("\n%s\n\n", s);
        tree = init_shell(s, a_env);
		if (tree)
		{
		/// your partner needs to free $p if it was expanded to nothing because of echo rr $p ff  it will print space in the place of $p instead it should do nothing
		// printf("\n%s\n\n", tree->cmd->cmd[1]);
        // printf("");
        // debug_tree(tree);
        if (prepare_heredoc(tree, envp) == -1)
            continue;
        // if(execute(tree, &env) == 1)
        // {
        //     free_tree(tree);
        //     // return (1);
		// }
		ret = execute(tree, &env);
		//you need to telll your partner to update the exit status for the syntax error
		printf("%d\n", ret);
		printf("%d\n", env->exit_s);
    	free_tree(tree);
		free_arr(a_env);
		}
    }
    return (ret);
}
