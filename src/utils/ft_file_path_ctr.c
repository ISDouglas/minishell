/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_file_path_ctr.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:16:28 by layang            #+#    #+#             */
/*   Updated: 2025/05/11 14:35:42 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_file_ctr(int fd, int he_fd, char	*msg, t_mini	*mi)
{
	if (fd == -1 || he_fd == -1)
	{
		if (msg)
			perror(msg);
		ft_free_mini(mi, 1);
		if (he_fd == -1)
			get_next_line(-1);
		exit(1);
	}
}

/* char	*ft_check_path_validity(t_mini *mini, char *path)
{
	if (!ft_strcmp(path, "") || !ft_strchr(path, '/')
		|| access(path, F_OK) == -1)
	{
		free(path);
		ft_putstr_fd(mini->cmd_array[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		ft_free_mini(mini, 1);
		exit(127);
	}
	else if (access(path, X_OK) == -1)
	{
		ft_putstr_fd(mini->cmd_array[0], 2);
		ft_putstr_fd(": permission denied\n", 2);
		ft_free_mini(mini, 1);
		free(path);
		exit(126);
	}
	return (path);
} */

char	*ft_check_path_validity(t_mini *mini, char *path)
{
	struct stat	path_stat;	

	if (!ft_strcmp(path, "") || !ft_strchr(path, '/')
		|| access(path, F_OK) == -1)
	{
		free(path);
		ft_putstr_fd(mini->cmd_array[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		ft_free_mini(mini, 1);
		exit(127);
	}
	if (stat(path, &path_stat) != 0)
		return (perror("Unable to get file information"), NULL);
	else if (access(path, X_OK) == -1 || S_ISDIR(path_stat.st_mode))
	{
		ft_putstr_fd(mini->cmd_array[0], 2);
		if (S_ISDIR(path_stat.st_mode))
			ft_putstr_fd(": Is a directory\n", 2);
		else
			ft_putstr_fd(": permission denied\n", 2);
		ft_free_mini(mini, 1);
		free(path);
		exit(126);
	}
	return (path);
}

void	safe_exit_child(t_mini	*mini, char	*msg, int code)
{
	dup2(mini->stdout_fd, STDOUT_FILENO);
	if (msg)
		printf("%s\n", msg);
	dup2(mini->log_fd, 1);
	ft_free_mini(mini, 1);
	exit(code);
}
