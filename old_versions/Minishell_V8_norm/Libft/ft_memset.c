/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:19:21 by nimorel           #+#    #+#             */
/*   Updated: 2024/11/08 14:12:55 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((unsigned char *)s)[i] = (unsigned char)c;
		i++;
	}
	return (s);
}
/*La  fonction  memset() remplit les n premiers octets de la zone mémoire
       pointée par s avec l’octet c.*/
/*	   
int main(void)
{
    char str[10] = "Hello";
	// int 97 = char 'a'
	ft_memset(str, 97, 5);
    printf("%s\n", str);

    return (0);
}*/