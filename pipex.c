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

char	*check_path(char **env, char *cmd)
{
	int		i;
	char	**path;
	char	*temp;

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

void	exec_pid(t_var *var, int *pipefd, int entry, int output)
{
	char	**cmd;
	char	*path;

	if (dup2(entry, STDIN_FILENO) == -1)
		return (ft_putstr_fd("Error dup2.", 2), exit(1));
	if (dup2(output, STDOUT_FILENO) == -1)
		return (ft_putstr_fd("Error dup2.", 2), exit(1));
	close(output);
	close(entry);
	close(pipefd[0]);
	close(pipefd[1]);
	cmd = ft_split(var->cmd, ' ');
	path = check_path(var->env, cmd[0]);
	if (path == NULL)
	{
		ft_putstr_fd("pipex: command not found: ", 2);
		ft_putstr_fd(cmd[0], 2);
		exit(EXIT_FAILURE);
	}
	execve(path, cmd, var->env);
	exit(EXIT_FAILURE);
}

int	setup_pid(int entryfd, int outputfd, t_var *var, char **argv)
{
	int		pipefd[2];
	pid_t	pid;
	int		i;

	i = 0;
	entryfd = set_entryfd(entryfd, argv, i);
	while (++i <= var->ncmd)
	{
		if (pipe(pipefd) == -1)
			return (ft_putstr_fd("Error pipe.", 2), 1);
		outputfd = pipefd[1];
		if (i == var->ncmd)
			outputfd = set_outputfd(outputfd, argv, i);
		if (entryfd != -1 && outputfd != -1)
			pid = fork();
		var->cmd = argv[i + 1];
		if (pid == 0)
			exec_pid(var, pipefd, entryfd, outputfd);
		close(entryfd);
		close(outputfd);
		entryfd = pipefd[0];
		wait(NULL);
	}
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	int		entryfd;
	int		outputfd;
	t_var	*var;

	var = NULL;
	entryfd = 0;
	outputfd = 0;
	if (argc < 5)
	{
		ft_putstr_fd("Not enought argument.", 2);
		return (1);
	}
	var = init_var(var, env, argc);
	if (setup_pid(entryfd, outputfd, var, argv) == 1)
		return (1);
	close(entryfd);
	close(outputfd);
	free(var);
	return (0);
}
