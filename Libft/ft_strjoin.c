/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:28:31 by nimorel           #+#    #+#             */
/*   Updated: 2025/05/10 16:02:18 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len;
	char	*dest;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2);
	dest = malloc(sizeof(char) * (len + 1));
	if (dest == NULL)
		return (NULL);
	i = 0;
	while (i < ft_strlen(s1))
	{
		dest[i] = s1[i];
		i++;
	}
	j = 0;
	while (j < ft_strlen(s2))
		dest[i++] = s2[j++];
	dest[i] = '\0';
	return (dest);
}

char	*ft_strjoin_free(char const *s1, char const *s2)
{
	char	*dest;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	dest = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (dest == NULL)
		return (NULL);
	i = -1;
	while (++i < ft_strlen(s1))
		dest[i] = s1[i];
	j = -1;
	while (++j < ft_strlen(s2))
		dest[i + j] = s2[j];
	dest[i + j] = '\0';
	free((void *)s1);
	free((void *)s2);
	return (dest);
}
/*int	main(void)
{
	char *s1 = "Hello ";
    char *s2 = "world !";
	char *res = ft_strjoin(s1, s2);
	printf("res: %s\n", res);
	free(res);
	return(0);
}*/
