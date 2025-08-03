/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rroundi <rroundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:33:22 by rroundi           #+#    #+#             */
/*   Updated: 2025/08/02 22:24:40 by rroundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    exported_env(t_env  *env)
{
    t_env   *curr;

    if (!env)
        return ;
    curr = env;
    while (curr != NULL)
    {
        if (!curr->value)
            printf("declare -x %s\n", curr->key);
        else
            printf("declare -x %s=\"%s\"\n", curr->key, curr->value);
        curr = curr->next;
    }
}

int if_valid(char *av)
{
    int i;

    i = 0;
    if (ft_isalpha(av[i]) == 0 && av[i] != '_')
        return (0);
    i++;
    while (av[i] && av[i] != '=')
    {
        if (ft_isalnum(av[i]) == 0 && av[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

char    **get_values(char   *av)
{
    char    **div;
    char    *a_equa;

    div = malloc(sizeof(char *) * 3);
    if (!div)
        return(NULL);
    a_equa = ft_strchr(av, '=');
    div[0] = ft_substr(av, 0, sublen(av));
    div[1] = ft_substr(a_equa, 1, ft_strlen(a_equa) - 1);
    div[2] = NULL;
    return (div);
}


// you need to expand variables if they needed expansion before calling the export function
int ft_export(char **av, t_env **c_env)
{
    t_env   *sorted;
    int i;
    int ret;
    char **div;

    ret = 0;
    i = 1;
    if (!av[1])
        return ( sorted = sort(*c_env), exported_env(sorted), 0);
    while (av[i])
    {
        if (!if_valid(av[i]))
            ret = error_msg("export", "is not a valid indentifier", av[i]);
        else if (ft_strchr(av[i],'=') != NULL)
        {
            div = get_values(av[i]);
            set_env_value(c_env, div[0], div[1]);
            free_arr(div);
        }
        else
            set_env_value(c_env, av[i],NULL);
        i++;
    }
    return (ret);
}
