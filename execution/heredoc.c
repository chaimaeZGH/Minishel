/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rroundi <rroundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:58:37 by rroundi           #+#    #+#             */
/*   Updated: 2025/08/04 21:06:48 by rroundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_sighere(int sig)
{
	(void)sig;
	printf("\n");
	exit(1);
}

char	*generate_filename(int file)
{
	char	*nbr;
	char	*str;

	nbr = ft_itoa(file);
	str = ft_strjoin("/tmp/heredoc_", nbr);
	free(nbr);
	return (str);
}

void	if_succes(char *line, char *file_name, int fd)
{
	free(line);
	free(file_name);
	close(fd);
	exit(0);
}

void	write_in_file(t_tokenlist *curr, char *file, char **env, int exit_s)
{
	int		fd;
	char	*line;
	char	*expanded_line;

	fd = open (file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		no_fd(file);
	while (1)
	{
		signal(SIGINT, handle_sighere);
		line = readline("> ");
		if (!line || ft_strcmp(line, curr->content) == 0)
			break ;
		if (curr->expnd == true)
		{
			expanded_line = expand_content(line, env, exit_s);
			free(line);
			line = expanded_line;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	if_succes(line, file, fd);
}

int	heredoc_redir(t_tokenlist *curr, char **env, int exit_s)
{
	static int		file;
	char			*file_name;
	pid_t			pid;
	int				status;
	struct termios	term;

	curr->fd = file;
	file_name = generate_filename(file++);
	tcgetattr(STDIN_FILENO, &term);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
		write_in_file(curr, file_name, env, exit_s);
	waitpid(pid, &status, 0);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
	{
		signal(SIGINT, handle_sigint);
		return (free(file_name), -1);
	}
	free(file_name);
	return (0);
}
