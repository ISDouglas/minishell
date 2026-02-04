/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 10:57:41 by nimorel           #+#    #+#             */
/*   Updated: 2025/05/10 18:40:31 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

t_env	*ft_dup_env(t_env	*env)
{
	t_env	*new_env;
	t_env	*cur_new;
	t_env	*new_node;

	new_env = NULL;
	cur_new = NULL;
	while (env)
	{
		new_node = ft_create_env_node(env->name, env->value);
		if (!new_node)
		{
			ft_free_env(&new_env);
			return ((void)ft_link_status("dup sub env failed", 1), NULL);
		}
		if (!new_env)
			new_env = new_node;
		else
			cur_new->next = new_node;
		cur_new = new_node;
		env = env->next;
	}
	return (new_env);
}

int	stamp_layer_env(t_mini	*mini)
{
	t_token	*cur;
	int		max;

	cur = mini->lexer;
	max = 0;
	while (cur)
	{
		if (cur->par_n > max)
			max = cur->par_n;
		cur = cur->next;
	}
	mini->layer = max + 1;
	if (mini->layer > 1)
	{
		mini->sub_env = malloc(sizeof(t_env *) * (mini->layer - 1));
		if (!mini->sub_env)
			return (perror("malloc env array"), 2);
		ft_memset(mini->sub_env, 0, sizeof(t_env *) * (mini->layer - 1));
	}
	return (0);
}

int	mlc_cpid(t_mini	*mini, int size)
{
	mini->cpid = malloc(sizeof(int) * size);
	if (!mini->cpid)
		return ((void)ft_link_status("malloc cpid failed", 1), 0);
	return (1);
}

/* int	dup_sub_shell(t_mini	*mini, int i)
{
	int	layer;
	
	if ((i - 2 >= 0 && next_type(mini, i - 2) == PIPE)
		|| next_type(mini, i) == PIPE)
	{
		if (i == 0 || (i - 2 >= 0 && next_type(mini, i - 2) != PIPE))
			layer = mini->exe_tab[i]->par_n;
		else
		{
			if (mini->exe_tab[i - 2]->par_n > mini->exe_tab[i]->par_n)
				layer = mini->exe_tab[i - 2]->par_n;
			else
				layer = mini->exe_tab[i]->par_n
		}	
	}
	else
		layer = mini->exe_tab[i]->par_n;
	if (layer > 0)
		mini->sub_env[layer - 1] = ft_dup_env();
	return (layer);
} */

//printf("\n>>> layer is : %d\n", layer);
int	dup_sub_shell(t_mini	*mini, int i)
{
	int	layer;
	int	n;

	if ((i - 2 >= 0 && next_type(mini, i - 2) == PIPE)
		&& mini->exe_tab[i - 2]->par_n > mini->exe_tab[i]->par_n)
		layer = mini->exe_tab[i - 2]->par_n;
	else
		layer = mini->exe_tab[i]->par_n;
	n = 0;
	printf("\n>>> layer is : %d\n", layer);
	while (n < layer)
	{
		if (!mini->sub_env[n])
		{
			if (n == 0)
				mini->sub_env[n] = ft_dup_env(mini->env);
			else
				mini->sub_env[n] = ft_dup_env(mini->sub_env[n - 1]);
			printf("\n  > layer add : sub_env[%d]\n", n);
		}
		if (!mini->sub_env[n])
			return (-1);
		n++;
	}
	return (layer);
}

int	is_delimiter(char sign, t_token	*tokens)
{
	static int	is;
	t_token		*cur;

	if (!tokens)
	{
		if (sign == 'w')
		{
			is = 0;
			return (is);
		}
		if (sign == 'c')
			return (is);
	}
	cur = tokens;
	while (cur->next)
		cur = cur->next;
	if (cur->type == HEREDOC)
		is = 1;
	else
		is = 0;
	return (is);
}
