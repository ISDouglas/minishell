/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel <marvin@42.fr> >          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 17:59:51 by nimorel           #+#    #+#             */
/*   Updated: 2024/11/14 10:04:50 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_nblen(long n)
{
	int	len;

	len = 1;
	if (n < 0)
		n = -n;
	while (n >= 10)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

static char	*ft_strnew(int len)
{
	char	*dest;

	dest = (char *)malloc(sizeof(char) * (len + 1));
	if (dest == NULL)
		return (NULL);
	dest[len] = '\0';
	return (dest);
}

char	*ft_itoa(int n)
{
	long	nb;
	int		len;
	char	*res;

	nb = n;
	len = ft_nblen(nb);
	if (nb < 0)
		len++;
	res = ft_strnew(len);
	if (res == NULL)
		return (NULL);
	if (nb < 0)
	{
		res[0] = '-';
		nb = -nb;
	}
	while (len > 0 && nb != 0)
	{
		res[--len] = (nb % 10) + '0';
		nb = nb / 10;
	}
	if (n == 0)
		res[0] = '0';
	return (res);
}
/*int	main(void)
{
	printf("%s\n", ft_itoa(-12));
	return (0);
}*/
