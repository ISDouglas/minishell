/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel <marvin@42.fr> >          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 16:59:36 by nimorel           #+#    #+#             */
/*   Updated: 2024/11/14 10:19:29 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *s1, const char *s2, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	len2;

	i = 0;
	len2 = ft_strlen(s2);
	if (len2 == 0)
		return ((char *)s1);
	while (s1[i] != '\0' && i < len)
	{
		j = 0;
		while (s1[i + j] != '\0' && s2[j] != '\0'
			&& s1[i + j] == s2[j] && i + j < len)
			j++;
		if (j == len2)
			return ((char *)s1 + i);
		i++;
	}
	return (NULL);
}
/*
int main(void)
{
   char *string1 = "Hello World";
   char *string2 = "tutu";
   char *result;
   int len;

   len = 25;
   result = ft_strnstr(string1,string2, len);
   if (result != NULL) {
        printf("sous-chaîne trouvee : %s\n", result);
    } else {
        printf("sous chaine non trouvee\n");
    }

    return (0);
}*/
