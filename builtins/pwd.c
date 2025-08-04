/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rroundi <rroundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:54:04 by rroundi           #+#    #+#             */
/*   Updated: 2025/08/04 19:54:21 by rroundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("PWD ERROR :");
		return (1);
	}
	ft_putendl_fd(cwd, 1);
	free(cwd);
	return (0);
}
