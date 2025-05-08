/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel <marvin@42.fr> >          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 09:52:43 by nimorel           #+#    #+#             */
/*   Updated: 2024/11/12 14:03:59 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*dest;

	i = 0;
	dest = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (dest == NULL)
		return (NULL);
	if (s && f)
	{
		while (s[i] != '\0')
		{
			dest[i] = f(i, s[i]);
			i++;
		}
	}
	dest[i] = '\0';
	return (dest);
}
/*static char    ft_upper(unsigned int i, char c)
{
    (void)i;
    return (c - 32);
}
int     main(void)
{
    char    *str = "hello";
    char    *result;

    result = ft_strmapi(str, &ft_upper);
    if (result == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        return (1);
    }

    printf("%s\n", result);
    free(result);

    return (0);
}*/
