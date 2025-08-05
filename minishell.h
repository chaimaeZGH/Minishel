/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rroundi <rroundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/08/05 22:39:13 by rroundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <sys/wait.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "./libft/libft.h"
# include <limits.h>
# include <fcntl.h>
# include <errno.h>
# include <signal.h>
# include <termios.h>

extern int	g_sig;

typedef enum a_type_list
{
	comnd,
	PIPE,
	INredirection,
	OUTredirection,
	HEREdocument,
	OUTappend
}	t_type_list;

typedef struct s_token
{
	char			*content;
	struct s_token	*prev;
	struct s_token	*next;
	bool			expnd;
	t_type_list		type;
	int				fd;
}					t_token;

typedef struct s_check
{
	int			i;
	t_token		*str;
}				t_check;

typedef struct s_st
{
	int		i;
	char	**str;
}				t_st;

typedef struct s_cmdlist
{
	int		here_doc_fd;
	char	**cmd;
	t_token	*args;
	t_token	*out;
	t_token	*in;
}				t_cmdlist;

typedef struct s_combo
{
	int		i;
	char	*str;
}				t_combo;

typedef struct s_char
{
	char		*str;
	char		*one;
}					t_char;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
	int				exit_s;
}					t_env;

typedef struct s_tree_list
{
	t_type_list			type;
	t_cmdlist			*cmd;
	struct s_tree_list	*right;
	struct s_tree_list	*left;
}						t_tree;

bool		copy_split(int *i, t_cmdlist *cmd, t_token *args);
bool		copy_rest(int *i, t_cmdlist *cmd, t_token *args);
bool		copy_args_to_cmd(t_cmdlist *cmd, t_token *args, int count);
void		fill_cmd_from_args(t_cmdlist *cmd);
void		fill_double_point(t_tree *tree);
void		free_cmd(char **cmd);
void		free_s_cmd(t_cmdlist *cmd);
void		free_tree(t_tree *tree);
t_token		*last_pipe(t_token *head);
void		remove_nodes(t_token *last_p, t_tree *tree, t_token **head);
void		fill_tree(t_cmdlist *cmd, t_token *original);
void		fill_tree(t_cmdlist *cmd, t_token *original);
t_token		*ft_lstnewnn(void *content, t_type_list ttype);
t_tree		*create_tree_node(void);
t_cmdlist	*create_cmd_node(t_tree *tree);
t_tree		*create_tree(t_token **head);
void		check_remove_quots(t_tree *root);
void		remove_quots(t_tree *root);
int			ft_count(char *s, char c);
int			count_args(t_token *args);
void		free_cmd_array(char **cmd, int i);
void		check_for_expend(t_tree *root, char **env, int exit_s);
void		check_string(char *old, int *i, int *j);
void		fill_string(char *old, char *new);
void		deleet_quots(t_token *str);
void		call_remove_quotes(t_token *current);
int			check_quotes(char *str);
int			initial_new_list(t_token *tkn, char **ev, t_token **lt, int et_s);
t_token		*handle_args(t_token *token, char **env, int exit_s );
void		check_expend_heardoc(t_token *current_in, char **env, int exit_s);
void		expend_check(t_tree *root, char **env, int exit_s);
void		merg_last_with_one(t_token **lst);
void		handle_operators(int *i, char *line, t_token **head);
int			handle_regular_cmd(char *line, int *i, t_token **head);
int			handle_quotes(char *line, int *i, t_token **head, char **ll);
int			process_line_part(char *line, int *i, t_token **head, char **ll);
void		popul_list(t_token *head, void *content);
t_token		*ft_lstnewn(void *content);
t_token		*ft_lstlastn(t_token *lst);
void		ft_lstadd_backn(t_token **lst, t_token *new);
void		process_expend_content(t_token *token, char **env, int exit_s);
char		*get_env_value(char **env, char *name);
char		*ft_strndup(char *s, size_t n);
char		*handle_special_cases(char *str, int *index, int exit_s);
char		*expand_variable(char *str, int *index, char **env, int exit_s);
int			append_char(char **result, char c);
int			expand_var(char *content, int *i, char **result, t_st *env_ex);
int			handle_quotes_expend(char c, char *quote_char, int *i);
int			process_char(t_char *strac, int *i, char **result, t_st	*env_ex);
char		*expnd_cont(char *content, char **env, int exit_s);
char		*expand_content(char *content, char **env, int exit_s);
int			split_line(char *line, t_token **head);
void		change_quots(t_combo *new, int j, char quote_char);
void		process_token_content(t_token *token);
void		replace_eache_node(t_tree *root);
void		replace_quotes(t_tree *root);
void		free_node(t_token *node);
void		add_new_conten(t_token *current, t_token *prevv, char *new_c);
char		*map_type(t_type_list type);
void		print_nodes(t_token *head);
void		free_list(t_token *head);
void		ft_error_msg(int errnum);
int			syntax_erreur(t_token *head);
int			chaeck_content(t_token *node);
void		continue_merge(t_token **head);
void		merge_file_cmd(t_token **head);
void		three_lines(t_check *strackt, int i, int j, char *cmd);
t_check		*extract_cmd(char *line, int i);
void		skip_whitespaces(int *i, char *line);
void		merge_this_with_next(t_token **lst, char *ll);
t_tree		*init_shell(char *s, char **env, t_env *envp);
// execution
int			ft_echo(char **av);
int			ft_env(t_env *env);
int			ft_exit(char **av);
int			ft_cd(t_env **c_env, char **av);
int			ft_export(char **av, t_env **c_env);
int			ft_pwd(void);
int			ft_unset(t_env **env, char **av);
int			print_error(char *cmd, char *msg);
int			error_msg(char *cmd, char *msg, char *av);
t_env		*copy_env_list(t_env *env);
int			sublen(char *av);
t_env		*find_key(t_env *head, char *key);
void		set_env_value(t_env **head, char *key, char *value);
t_env		*copy_env(char **env);
void		print_env_list(t_env *head);
t_env		*sort(t_env *env);
int			if_valid(char *av);
char		**to_array(t_env *env);
char		**get_path(char *cmd, t_env *env);
void		free_arr(char **arr);
char		*true_path(char *cmd, t_env *env);
int			exec_bin(char **cmd, t_env *env);
int			is_builtin(t_tree *tree);
int			execute(t_tree *tree, t_env **env);
int			execute_builtins(t_tree *tree, t_env **env, int flag);
int			execute_pipe(t_tree *tree, t_env **env);
int			execute_out_redirection(t_tree *tree, char *fn, t_env **env);
int			execute_in_redirection(t_tree *tree, char *fn, t_env **env);
int			execute_with_redirections(t_tree *tree, t_env **env);
int			ft_strcmp(const char *s1, const char *s2);
int			heredoc_redir(t_token *curr, char **env, int exit_s);
char		*generate_filename(int file);
int			prepare_heredoc(t_tree *tree, char **env, int exit_s);
int			handle_exec(char **cmd);
int			ft_call_env(t_env *env, char **cmd);
void		handle_sigint(int sig);
void		setup_signals(void);
void		free_env_list(t_env *head);
void		no_fd(char	*file_name);
int			in_redir_call(t_tree *tree, int *s_stdin, t_env **env);
int			in_redir(t_token *in, int *s_stdin);
int			for_heredoc(t_token *curr);
int			for_inredir(t_token *curr, t_token *in);
t_env		*sort(t_env *env);
t_env		*create_node(char *key, char *value);
void		add_front(t_env **head, t_env *new_node);
void		for_signals(t_env *env, char *s, char **a_env);

#endif
