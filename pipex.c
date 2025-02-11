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

int	def_output(int i, char **argv, int *pipefd)
{
	int output;

	if (i == strlen_cmd(argv))
		output = open(argv[i + 2], O_CREAT | O_TRUNC | O_WRONLY, 00644);
	else
		output = pipefd[1];
	return (output);
}

void	exec_pid(t_var *var, int *pipefd, int entry, int output)
{
	char	**cmd;
	char	*path;

	dup2(entry, STDIN_FILENO);
	dup2(output, STDOUT_FILENO);
	close(output);
	close(entry);
	close(pipefd[0]);
	close(pipefd[1]);
	cmd = ft_split(var->cmd, ' ');
	path = check_path(var->env, cmd[0]);
	if (path == NULL)
		exit(EXIT_FAILURE);
	execve(path, cmd, var->env);
	exit(EXIT_FAILURE);
}

t_var *init_var(t_var *var, char **env, int argc)
{
	var = malloc(sizeof(t_var) * 1);
	var->env = env;
	var->ncmd = argc - 3;
	return (var);
}

int main(int argc, char **argv, char **env)
{
	int pipefd[2];
	pid_t pid;
	int entryfd;
	int outputfd;
	int i = 1;
	t_var *var;

	var = NULL;
	if (argc < 5)
	{
		return (1);
	}
	var = init_var(var, env, argc);
	entryfd = open(argv[i], O_RDONLY);
	while (i <= var->ncmd)
	{
		if (pipe(pipefd) == -1)
			return (0);
		outputfd = pipefd[1];
		if (i == var->ncmd)
			outputfd = open(argv[i + 2], O_CREAT | O_TRUNC | O_WRONLY, 00644);
		pid = fork();
		var->cmd = argv[i + 1];
		if (pid == 0)
		{
			exec_pid(var, pipefd, entryfd, outputfd);
		}
		close(entryfd);
		close(outputfd);
		entryfd = pipefd[0];
		i++;
		// waitpid(pid, NULL, 0);
	}
	return (0);
}
