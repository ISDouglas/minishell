/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel <marvin@42.fr> >          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:44:42 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/22 18:52:11 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	pass_space_sign(const char **str, int *sign)
{
	while (**str == ' ' || (**str >= 9 && **str <= 13))
		(*str)++;
	if (**str == '+' || **str == '-')
	{
		if (**str == '-')
			*sign = -1;
		(*str)++;
	}
}

int	ft_is_numeric(const char *str)
{
	int	i;

	i = 0;
	while (*str == ' ')
		str++;
	if (*str == '+' || *str == '-')
		str++;
	while (ft_isdigit(*str))
	{
		str++;
		i++;
	}
	while (*str == ' ')
		str++;
	if (*str == '\0' && i > 0)
		return (1);
	return (0);
}

int	ft_atoi(const char *nptr)
{
	int	sign;
	int	i;
	int	res;

	i = 0;
	sign = 1;
	res = 0;
	while (nptr[i] == ' ' || nptr[i] == '\n' || nptr[i] == '\t'
		|| nptr[i] == '\v' || nptr[i] == '\f' || nptr[i] == '\r')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -sign;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		res = res * 10 + (nptr[i] - 48);
		i++;
	}
	return (res * sign);
}

int	ft_atoll_code(const char *str, long long *result)
{
	int			sign;
	long long	num;
	int			digit;

	num = 0;
	sign = 1;
	pass_space_sign(&str, &sign);
	if (!ft_isdigit(*str))
		return (0);
	while (ft_isdigit(*str))
	{
		digit = *str - '0';
		if (sign == 1 && (num > (LLONG_MAX - digit) / 10))
			return (0);
		if (sign == -1 && (-num < (LLONG_MIN + digit) / 10))
			return (0);
		num = num * 10 + digit;
		str++;
	}
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str != '\0')
		return (0);
	*result = num * sign;
	return (1);
}

/*
int	main(void)
{
	printf("%d\n",ft_atoi(" -1234ab567"));
}*/
