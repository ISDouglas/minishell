/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:49:06 by nimorel           #+#    #+#             */
/*   Updated: 2024/11/11 11:23:24 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	i = 0;
	if (s && f)
	{
		while (s[i] != '\0')
		{
			f(i, &s[i]);
			i++;
		}
	}
}
/*static void    ft_upper(unsigned int i, char *c)
{
    (void)i;
    *c = *c - 32;
}
int     main(void)
{
    char    str[5] = "hello";

    ft_striteri(str, &ft_upper);
	printf("%s\n", str);
    return (0);
}*/
