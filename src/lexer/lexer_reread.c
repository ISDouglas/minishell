/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_reread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 04:16:37 by layang            #+#    #+#             */
/*   Updated: 2025/05/10 15:07:53 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static void	reglob_tokens(char	*word, t_token **t_n, char	*s_g, size_t j)
{
	if (!reget_files(word, t_n, s_g, j))
		ft_add_token(t_n, ft_create_token(word, WORD, j));
	free(word);
}

//	printf("\n..REREAD: create single token: %s\n", word);
static void	reblock_one_token(char	*word, size_t j, t_mini	*mini, int in)
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
		reblock_one_token(word, j, mini, in);
	else
		reglob_tokens(word, &mini->exe_tab[in], sign_glob, j);
	free(sign_glob);
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
		{
			(void)is_delimiter('w', mini->exe_tab[i]);
			ft_rehandle_block(mini->input, &s, mini, i);
		}
	}
	ft_stamp_token(NULL, &mini->exe_tab[i], pa_n);
	printf("\n**  tab-line %d after REREAD:\n  ", i);
	ft_print_token(mini->exe_tab[i]);
}

void	reread_pipe_chain(t_mini	*mini, int i)
{
	int	n_pip;

	n_pip = i;
	if (i == 0 || (i - 2 >= 0 && next_type(mini, i - 2) != PIPE))
	{
		while (next_type(mini, n_pip) == PIPE)
		{
			ft_tabn_reread(mini, mini->exe_tab[n_pip]->start, n_pip);
			n_pip = n_pip + 2;
		}
		if (n_pip < mini->tab_size)
			ft_tabn_reread(mini, mini->exe_tab[n_pip]->start, n_pip);
	}
}
