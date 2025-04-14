/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:30:08 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/09 11:47:15 by layang           ###   ########.fr       */
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

static char	*ft_cd_path(t_token *tokens, t_env *env)
{
	if (!tokens->next || ft_strcmp(tokens->next->value, "~") == 0)
		return (ft_getenv(env, "HOME"));
	if (ft_strcmp(tokens->next->value, "-") == 0)
		return (ft_getenv(env, "OLDPWD"));
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
		return (perror("cd: copy cwd failed"), FAILURE);
	path = ft_cd_path(tokens, env);
	if (!path || chdir(path) != 0)
		return (perror("cd"), free(oldpwd), FAILURE);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		if (ft_update_var(env, ft_strdup("OLDPWD"), oldpwd) == FAILURE
			|| ft_update_var(env, ft_strdup("PWD"), ft_strdup(cwd)) == FAILURE)
			return (perror("cd: update OLDPWD and PWD failed"), FAILURE);
	}
	else
		free(oldpwd);
	return (printf("cd success.\n"), SUCCESS);
}
