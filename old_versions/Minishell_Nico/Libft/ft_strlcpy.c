/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:44:12 by nimorel           #+#    #+#             */
/*   Updated: 2024/11/11 17:17:06 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size == 0)
		return (ft_strlen(src));
	while (src[i] != '\0' && i < (size - 1))
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen(src));
}
/*La strlcpyfonction () copie jusqu'à size - 1 caractères de la chaîne src
terminée par NUL vers dst , terminant le résultat par NUL.
strlcpy renvoie la longueur totale de la chaîne qu'elle a essaye de creer
Pour strlcpy(), cela signifie la longueur de src*/
/*int main(void)
{
	char *src = "Hello";
	char dest[50];

	printf("%ld\n", ft_strlcpy(dest, src, 5));
	return(0);
}*/
