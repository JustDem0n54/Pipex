/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrontard <nrontard@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:17:53 by nrontard          #+#    #+#             */
/*   Updated: 2025/02/13 15:38:51 by nrontard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <sys/wait.h>
# include "libft/libft.h"

typedef struct s_var
{
	char	*cmd;
	char	**env;
	int		ncmd;
	int		i;
}	t_var;

void	free_split(char **str);
int		strlen_cmd(char **argv);
t_var	*init_var(t_var *var, char **env, int argc);
char	*check_path(char **env, char *cmd);
void	exec_pid(t_var *var, int *pipefd, int entry, int output);
int		setup_pid(int entryfd, int outputfd, t_var *var, char **argv);
int		set_outputfd(int outputfd, char **argv, int i);
int		set_entryfd(int entryfd, char **argv, int i);

#endif