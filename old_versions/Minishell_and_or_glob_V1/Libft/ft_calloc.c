/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:40:55 by nimorel           #+#    #+#             */
/*   Updated: 2024/11/15 14:34:53 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*dest;

	if (nmemb == 0 || size == 0)
		return (malloc(0));
	if (nmemb > (nmemb * size / size))
		return (NULL);
	dest = malloc(nmemb * size);
	if (dest == NULL)
		return (NULL);
	ft_memset(dest, 0, nmemb * size);
	return (dest);
}
/*
int main(void)
{
    size_t nmemb = 5;
    size_t size = sizeof(int);
    int *tab;
	size_t	i;

    i = 0;
	tab = (int *)ft_calloc(nmemb, size);
    if (tab == NULL)
    {
        printf("Allocation de mémoire échouée.\n");
        return (1);
    }
	while (i < nmemb)
    {
        printf("tab[%zu] = %d\n", i, tab[i]);
		i++;
    }

    // Libère la mémoire
    free(tab);
    return (0);
}*/
