/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:02:33 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/11 13:35:15 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_tokens(t_token **tokens)
{
	t_token	*tmp;

	//printf("\n\n**Free tokens:\n");
	if (!tokens || !*tokens)
	{
        return ;
    }
	while (*tokens)
	{
		tmp = *tokens;
		free(tmp->value);
		//printf("Free tokens, address: %p\n", tokens);
		free(tmp->cmd);
		*tokens = (*tokens)->next;
		free(tmp);
	}
	*tokens = NULL;
}

t_token	*ft_create_token(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	if (!token->value)
    {
        free(token);
        return (NULL);
    }
	token->type = type;
	token->infile = STDIN_FILENO;
	token->outfile = STDOUT_FILENO;
	token->cmd = NULL;
	token->next = NULL;
	return (token);
}

void	ft_add_token(t_token **tokens, t_token *new_token)
{
	t_token	*current;

	if (!new_token)
		return ;
	new_token->next = NULL; 
	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

t_token_type	ft_get_operator_type(char c, char next_c)
{
	if (c == '&' && next_c == '&')
		return (AND);
	if (c == '|' && next_c == '|')
		return (OR);
	if (c == '|')
		return (PIPE);
	if (c == '<' && next_c == '<')
		return (HEREDOC);
	if (c == '>' && next_c == '>')
		return (APPEND);
	if (c == '<')
		return (REDIRECT_IN);
	if (c == '>')
		return (REDIRECT_OUT);
	if (c == '$')
		return (ENV_VAR);
	return (WORD);
}

void	ft_handle_word(const char *input, size_t *i, t_token **tokens)
{
	int		start;
	int		len;
	char	*word;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && !ft_strchr("|<>", input[*i])) //lack &?
		(*i)++;
	len = *i - start;
	word = malloc(len + 1);
	if (!word)
		return ;
	ft_memcpy(word, &input[start], len);
	word[len] = '\0';
	ft_add_token(tokens, ft_create_token(word, WORD));
	free(word);
}
