
# ifndef MINISHELL_H
# define MINISHELL_H

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */


# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "./libft/libft.h"
# include <stdio.h> 
#include <readline/readline.h>
#include <readline/history.h>
#include "./libft/libft.h"
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>


typedef enum a_type_list
{
	comnd,
	PIPE,
	INredirection,
	OUTredirection,
	HEREdocument,
	OUTappend
}	t_type_list;

typedef struct s_tokenlist
{
	char				*content;
	struct s_tokenlist	*prev;
	struct s_tokenlist	*next;
	bool				expnd;
	t_type_list			type;
	int					fd;
}						t_tokenlist;

typedef struct s_check
{
	int			i;
	t_tokenlist	*str;
}				t_check;

typedef struct s_cmdlist
{
	int			here_doc_fd;
    char		**cmd;
    t_tokenlist	*args;
    t_tokenlist	*out;
    t_tokenlist	*in;
}				t_cmdlist;

typedef struct s_tree_list
{
	t_type_list			type;
	t_cmdlist			*cmd;
	struct s_tree_list	*right;
	struct s_tree_list	*left;
}						t_tree_list;

void		print_ast_topdown(t_tree_list *root);//DELEET
t_tree_list *create_tree(t_tokenlist **head);
t_tokenlist *ft_lstnewn(void *content);
t_tokenlist *ft_lstlastn(t_tokenlist *lst);
void		ft_lstadd_backn(t_tokenlist **lst, t_tokenlist *new);
void		free_list(t_tokenlist *head);
void		free_tree(t_tree_list *tree);
void print_nodes(t_tokenlist *head);
void		process_expend_content(t_tokenlist *token, char **env);




// execution
typedef struct s_env{
    char *key;
    char *value;
    struct s_env *next;
	int		exit_s;
}t_env;

//builtins

int		ft_echo(char    **av);
int		ft_env(t_env    *env);
int		ft_exit(char **av);
int 	ft_cd(t_env **c_env, char **av);
int 	ft_export(char **av, t_env **c_env);
int 	ft_pwd(void);
int 	ft_unset(t_env  **env, char **av);

//
int 	print_error(char *cmd, char *msg);
int 	error_msg(char *cmd, char *msg, char *av);
t_env   *copy_env_list(t_env *env);
int 	sublen(char *av);
t_env   *find_key(t_env *head, char *key);
void    set_env_value(t_env **head, char *key, char *value);
t_env   *copy_env(char **env);
void 	print_env_list(t_env *head);
t_env   *sort(t_env *env);
int 	if_valid(char *av);
char    **to_array(t_env    *env);
char    **get_path(char  *cmd, t_env  *env);
void    free_arr(char **arr);
char    *true_path(char  *cmd, t_env *env);
int     exec_bin(char   **cmd, t_env    *env);
int 	is_builtin(t_tree_list *tree);
int 	execute(t_tree_list *tree, t_env    **env);
int 	execute_builtins(t_tree_list *tree, t_env   **env, int flag);
int 	execute_pipe(t_tree_list *tree, t_env   **env);
int 	execute_out_redirection(t_tree_list *tree, char *fn, t_env **env);
int		execute_in_redirection(t_tree_list *tree, char *fn, t_env **env);
int 	execute_with_redirections(t_tree_list *tree, t_env **env);
int 	ft_strcmp(const char *s1, const char *s2);
int 	heredoc_redir(t_tokenlist  *curr, char	**env);
char    *generate_filename(int  file);
int		prepare_heredoc(t_tree_list *tree, char **env);
char 	*expand_content(const char *content, char **env);
int    handle_exec(char    **cmd);

#endif
