/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:58:40 by czghoumi          #+#    #+#             */
/*   Updated: 2025/06/24 07:06:04 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
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
#include <readline/readline.h>
#include <readline/history.h>
#include "./libft/libft.h"


typedef enum a_type_list{
	comnd,
	PIPE,
	INredirection,
	OUTredirection,
	HEREdocument,
	OUTappend
}	t_type_list;

typedef struct s_tokenlist
{
	char			*content;
	struct s_tokenlist      	*prev;
	struct s_tokenlist      	*next;
	t_type_list		type;
}	                t_tokenlist;


typedef struct s_check
{
	int i;
	t_tokenlist *str;
} t_check;

// newshit
// {
//     t_tokenlist in;
//     t_tokenlist out;
//     t_tokenlist args;
// }

typedef struct s_tree_list
{
	t_type_list             type;
	t_tokenlist      *cmd; //newshit
	struct s_tree_list      *right;
	struct s_tree_list      *left;
}                           t_tree_list;

t_tokenlist *ft_lstnewn(void *content);
t_tokenlist *ft_lstlastn(t_tokenlist *lst);
void		ft_lstadd_backn(t_tokenlist **lst, t_tokenlist *new);




#endif