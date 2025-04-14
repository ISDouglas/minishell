/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:30:08 by nimorel           #+#    #+#             */
/*   Updated: 2025/03/28 09:24:39 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		write(1, cwd, ft_strlen(cwd));
		write(1, "\n", 1);
		return (SUCCESS);
	}
	perror("pwd");
	return (FAILURE);
}

static char	*ft_cd_path(t_token *tokens)
{
	if (!tokens->next)
		return (getenv("HOME"));
	if (strcmp(tokens->next->value, "~") == 0)
		return (getenv("OLDPWD"));
	return (tokens->next->value);
}

int	ft_cd(t_token *tokens, t_env *env)
{
	char	cwd[PATH_MAX];
	char	*oldpwd;
	char	*path;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (perror("cd: getcwd failed"), FAILURE);
	oldpwd = ft_strdup(cwd);
	if (!oldpwd)
		return (FAILURE);
	path = ft_cd_path(tokens);
	if (!path || chdir(path) != 0)
		return (perror("cd"), free(oldpwd), FAILURE);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		if (ft_update_var(env, ft_strdup("OLDPWD"), oldpwd) == FAILURE
			|| ft_update_var(env, ft_strdup("PWD"), ft_strdup(cwd)) == FAILURE)
			return (FAILURE);
	}
	else
		free(oldpwd);
	return (SUCCESS);
}
