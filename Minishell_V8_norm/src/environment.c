/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 10:36:34 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/13 12:56:03 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_getenv(t_env	*env, const char	*name)
{
	t_env	*current;

	current = env;
	while (current != NULL)
	{
		if (ft_strcmp(current->name, name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	ft_free_env(t_env **env)
{
	t_env	*tmp;

	while (*env)
	{
		tmp = *env;
		*env = (*env)->next;
		if (tmp->name)
		{
			free(tmp->name);
			tmp->name = NULL;
		}
		if (tmp->value)
		{
			free(tmp->value);
			tmp->value = NULL;
		}
		free(tmp);
	}
	*env = NULL;
}

t_env	*ft_create_env_node(const char	*name, const char	*value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->name = ft_strdup(name);
	node->value = ft_strdup(value);
	if (!node->name || !node->value)
	{
		free(node);
		perror("node creation failed");
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

static t_env	*ft_init_env_process(t_env	**head, t_env	**cur, char	*envp)
{
	char	*equal_sign;
	t_env	*new_node;

	new_node = NULL;
	equal_sign = ft_strchr(envp, '=');
	if (!equal_sign)
		return (NULL);
	*equal_sign = '\0';
	new_node = ft_create_env_node(envp, equal_sign + 1);
	*equal_sign = '=';
	if (!new_node)
		return (NULL);
	if (!*head)
		*head = new_node;
	else
		(*cur)->next = new_node;
	*cur = new_node;
	return (new_node);
}

t_env	*ft_init_env(char	**envp)
{
	t_env	*head;
	t_env	*current;
	int		i;

	head = NULL;
	current = NULL;
	i = 0;
	while (envp[i])
	{
		if (!ft_init_env_process(&head, &current, envp[i]))
		{
			ft_free_env(&head);
			return (NULL);
		}
		i++;
	}
	return (head);
}
