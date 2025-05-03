/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_reread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 04:16:37 by layang            #+#    #+#             */
/*   Updated: 2025/05/03 07:16:49 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static void	reglob_token(char	*word, t_token **t_n, char	*s_g, size_t j)
{
	if (!reget_files(word, t_n, s_g, j))
		ft_add_token(t_n, ft_create_token(word, WORD, j));
	free(word);
}

//	printf("\n..REREAD: create single token: %s\n", word);
static void	ft_reblock_one(char	*word, size_t j, t_mini	*mini, int in)
{
	if (!word || (!ft_strcmp(word, "") && mini->input[j] == '$'))
	{
		if (word)
			free(word);
		return ;
	}
	ft_add_token(&mini->exe_tab[in], ft_create_token(word, WORD, j));
	free(word);
}

static void	ft_rehandle_block(const char *input, size_t *i, t_mini *mini,
				int in)
{
	char	*word;
	size_t	j;
	char	*sign_glob;

	j = *i;
	ft_block_init(&word, &sign_glob);
	while (input[*i] && !ft_isspace(input[*i]) && !ft_strchr("|<>&()",
			input[*i]))
	{
		if (input[*i] == '\'')
			word = ft_strjoin_free(word, get_squote_block(input, i));
		else if (input[*i] == '"')
			word = ft_strjoin_free(word, get_dquote_block(input, i, mini));
		else
		{
			renew_sign_glob(word, &sign_glob, 0);
			word = ft_strjoin_free(word, get_word_block(input, i, mini));
			renew_sign_glob(word, &sign_glob, 1);
		}
	}
	if (!word_has_glob(sign_glob))
		ft_reblock_one(word, j, mini, in);
	else
		reglob_token(word, &mini->exe_tab[in], sign_glob, j);
	free(sign_glob);
}

static void	assign_par_n(t_token **tab_n, int pa_n)
{
	t_token	*tmp;

	tmp = *tab_n;
	while (tmp)
	{
		tmp->par_n = pa_n;
		tmp = tmp->next;
	}
}

void	ft_tabn_reread(t_mini	*mini, size_t st, int i)
{
	int		pa_n;
	size_t	s;

	pa_n = mini->exe_tab[i]->par_n;
	s = st;
	ft_free_tokens(&mini->exe_tab[i]);
	while (mini->input[s])
	{
		if (ft_isspace(mini->input[s]))
			s++;
		else if (ft_strchr("|&()", mini->input[s]))
			break ;
		else if (ft_strchr("<>", mini->input[s]))
			ft_handle_operator(mini->input, &s, &mini->exe_tab[i]);
		else
			ft_rehandle_block(mini->input, &s, mini, i);
	}
	assign_par_n(&mini->exe_tab[i], pa_n);
	printf("\n**  tab %d after REREAD:\n  ", i);
	ft_print_token(mini->exe_tab[i]);
}
