/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   pipex.c											:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: nrontard <nrontard@student.42perpignan.	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/01/28 13:47:37 by nrontard		  #+#	#+#			 */
/*   Updated: 2025/02/05 16:58:33 by nrontard		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "pipex.h"

void	free_split(char **str)
{
	int i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

char	*check_path(char **env, char *cmd)
{
	int i;
	char **path;
	char *temp;

	if (ft_strchr(cmd, '/') != NULL)
	{
		if (access(cmd, F_OK) == 0)
			return (cmd);
	}
	i = 0;
	while (ft_strncmp(env[i], "PATH", 4) != 0)
		i++;
	path = ft_split(env[i] + 5, ':');
	i = 0;
	while (path[i] != NULL)
	{
		temp = ft_strnjoin(2, (char *[]){path[i], cmd}, "/");
		if (access(temp, F_OK) == 0)
			return (free_split(path), temp);
		free(temp);
		i++;
	}
	return (free_split(path), NULL);
}

int strlen_cmd(char **argv)
{
	int i;

	i = 0;
	while (argv[i])
		i++;
	return (i - 3);
}

int	def_entry(int i, char **argv, int *pipefd)
{
	int entry;

	if (i == 1)
		entry = open(argv[i], O_RDONLY);
	else
		entry = pipefd[0];
	return (entry);
}

int	def_output(int i, char **argv, int *pipefd)
{
	int output;

	if (i == strlen_cmd(argv))
		output = open(argv[i + 2], O_CREAT | O_TRUNC | O_WRONLY, 00644);
	else
		output = pipefd[1];
	return (output);
}

void	exec_pid1(char **argv, char **env, int *pipefd, int i)
{
	char	**cmd;
	char	*path;
	int		entry;
	int 	output;

	entry = def_entry(i, argv, pipefd);
	output = def_output(i, argv, pipefd);
	dup2(entry, STDIN_FILENO);
	dup2(output, STDOUT_FILENO);
	close(output);
	close(entry);
	close(pipefd[0]);
	close(pipefd[1]);
	cmd = ft_split(argv[i + 1], ' ');
	path = check_path(env, cmd[0]);
	if (path == NULL)
	{
		free(path);
		exit(EXIT_FAILURE);
	}
	execve(path, cmd, env);
}

int main(int argc, char **argv, char **env)
{
	int pipefd[2];
	pid_t pid1;
	pid_t pid2;
	int i = 1;

	if (argc != 5)
	{
		return (1);
	}
	if (pipe(pipefd) == -1)
		return (0);
	pid1 = fork();
	if (pid1 == 0)
	{
		exec_pid1(argv, env, pipefd, i);
	}
	i++;
	pid2 = fork();
	if (pid2 == 0)
	{
		exec_pid1(argv, env, pipefd, i);
	}
	return (0);
}
