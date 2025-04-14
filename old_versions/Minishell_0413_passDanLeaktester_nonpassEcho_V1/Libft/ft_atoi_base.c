/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel <marvin@42.fr> >          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 14:33:47 by nimorel           #+#    #+#             */
/*   Updated: 2025/03/06 15:04:40 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi_base(const char *str, char *base)
{
	int	i;
	int	res;
	int	sign;
	int	base_len;

	i = 0;
	res = 0;
	sign = 1;
	base_len = ft_strlen(base);
	if (base_len < 2 || base_len > 16)
		return (0);
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -sign;
		i++;
	}
	while (str[i] && ft_strchr(base, str[i]))
	{
		res = res * base_len + (ft_strchr(base, str[i]) - base);
		i++;
	}
	return (res * sign);
}
