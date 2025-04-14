/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 10:36:34 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/05 13:54:37 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_getenv(t_env *env, const char *name)
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

void	ft_free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}
}

t_env	*ft_create_env_node(const char *name, const char *value)
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

static t_env	*ft_init_env_process(t_env **head, t_env **current, char *envp)
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
		(*current)->next = new_node;
	*current = new_node;
	return (new_node);
}

t_env	*ft_init_env(char **envp)
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
			ft_free_env(head);
			return (NULL);
		}
		i++;
	}
	return (head);
}

/*t_env	*ft_init_env(char **envp)
{
	t_env	*head;
	t_env	*current;
	t_env	*new_node;
	char	*equal_sign;
	int		i;

	head = NULL;
	current = NULL;
	i = 0;
	while (envp[i])
	{
		equal_sign = ft_strchr(envp[i], '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			new_node = ft_create_env_node(envp[i], equal_sign + 1);
			*equal_sign = '=';
			if (!new_node)
			{
				ft_free_env(head);
				return (NULL);
			}
			if (!head)
				head = new_node;
			else
				current->next = new_node;
			current = new_node;
		}
		i++;
	}
	return (head);
}*/
