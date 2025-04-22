/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:39:57 by nimorel           #+#    #+#             */
/*   Updated: 2024/11/16 13:19:58 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t	i;

	if (dest == src || n == 0)
		return (dest);
	if (dest > src)
	{
		i = n;
		while (i > 0)
		{
			i--;
			((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
		}
	}
	else
	{
		i = 0;
		while (i < n)
		{
			((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
			i++;
		}
	}
	return (dest);
}
/*
int main() {
	// Différents tableaux pour les tests
	char src1[] = "Hello, world!";
	char src2[] = "Bonjour, monde!";
	char dst[30];

	// Test 1 : Copie simple sans overlap
	printf("Test 1 : Copie simple\n");
	ft_memmove(dst, src1, sizeof(src1));
	printf("dst = %s\n", dst);

	// Test 2 : Copie avec overlap (destination après source)
	printf("\nTest 2 : Copie avec overlap (destination après source)\n");
	ft_memmove(dst + 5, dst, 10);
	printf("dst = %s\n", dst);

	 // Test 3 : Copie avec overlap (destination avant source)
	printf("\nTest 3 : Copie avec overlap (destination avant source)\n");
	ft_memmove(dst, dst + 5, 10);
	printf("dst = %s\n", dst);

	// Test 4 : Copie de tailles différentes
	printf("\nTest 4 : Copie de tailles différentes\n");
	ft_memmove(dst, src2, sizeof(src2));
	printf("dst = %s\n", dst);

	return 0;
}*/
