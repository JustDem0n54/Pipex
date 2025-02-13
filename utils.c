/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrontard <nrontard@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:23:34 by nrontard          #+#    #+#             */
/*   Updated: 2025/02/13 17:16:12 by nrontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_split(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

int	strlen_cmd(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
		i++;
	return (i - 3);
}

t_var	*init_var(t_var *var, char **env, int argc)
{
	var = malloc(sizeof(t_var) * 1);
	var->env = env;
	var->ncmd = argc - 3;
	var->i = 0;
	return (var);
}

int	set_outputfd(int outputfd, char **argv, int i)
{
	outputfd = open(argv[i + 2], O_CREAT | O_TRUNC | O_WRONLY, 00644);
	if (outputfd == -1)
	{
		ft_putstr_fd(argv[i + 2], 2);
		ft_putstr_fd(": Permission denied", 2);
	}
	return (outputfd);
}

int	set_entryfd(int entryfd, char **argv, int i)
{
	if (i == 1)
	{
		entryfd = open("temp", O_RDONLY);
		if (entryfd == -1)
		{
			ft_putstr_fd("temp", 2);
			ft_putstr_fd(": No Such file or directory\n", 2);
		}
	}
	else if (i == 0)
	{
		entryfd = open(argv[i + 1], O_RDONLY);
		if (entryfd == -1)
		{
			ft_putstr_fd(argv[i + 1], 2);
			ft_putstr_fd(": No Such file or directory\n", 2);
		}
	}
	return (entryfd);
}
