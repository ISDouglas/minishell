/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel <marvin@42.fr> >          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 17:33:13 by nimorel           #+#    #+#             */
/*   Updated: 2024/11/12 14:03:13 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_count_words(const char *str, char c)
{
	int	i;
	int	cw;

	i = 0;
	cw = 0;
	while (str[i] != '\0')
	{
		while (str[i] == c && str[i] != '\0')
			i++;
		if (str[i] != c && str[i] != '\0')
			cw++;
		while (str[i] != c && str[i] != '\0')
			i++;
	}
	return (cw);
}

char	*ft_addword(const char *str, int start, int end)
{
	int		i;
	char	*word;

	i = 0;
	word = malloc(sizeof(char) * (end - start + 1));
	if (word == NULL)
		return (NULL);
	while (start < end)
		word[i++] = str[start++];
	word[i] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		j;
	int		len;
	char	**dest;

	i = 0;
	j = 0;
	dest = malloc(sizeof(char *) * (ft_count_words(s, c) + 1));
	if (dest == NULL)
		return (NULL);
	while (s[i] != '\0')
	{
		while (s[i] == c && s[i] != '\0')
			i++;
		len = 0;
		while (s[i + len] != '\0' && s[i + len] != c)
			len++;
		if (len > 0)
		{
			dest[j++] = ft_addword(s, i, i + len);
			i = i + len;
		}
	}
	dest[j] = NULL;
	return (dest);
}
/*int	main(void)
{
	char str[] = "Bleu;blanc;rouge";
	char c = ';';
	int i = 0;

	char **res = ft_split(str, c);
	if (res == NULL)
		return (1);
	while (res[i])
	{
		printf("%s\n", res[i]);
		i++;
	}
	i = 0;
	while (res[i])
	{
		free(res[i]);
		i++;
	}
	free(res);
	return (0);
}*/
