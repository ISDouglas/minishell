/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:56:44 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/01 15:57:37 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			i;

	i = 0;
	if (!dest || !src)
		return (0);
	while (i < n)
	{
		((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dest);
}
/*La  fonction memcpy() copie n octets depuis la zone mémoire src vers la
       zone mémoire dest. Les deux zones ne  doivent  pas  se  chevaucher*/
/*
int	main(void)
{
	char	*str = "Hello";
	char	dest[10];

	ft_memcpy(dest, str, 5);
	printf("chaine copiée : %s\n", dest);
	return(0);
}*/
