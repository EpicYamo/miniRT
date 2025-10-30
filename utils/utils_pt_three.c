/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pt_three.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 16:30:48 by aaycan            #+#    #+#             */
/*   Updated: 2025/10/30 23:47:46 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

static int	ft_isspacemod(int *i, const char *strwosp);

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (((unsigned char)s1[i] || (unsigned char)s2[i]) && i < n)
	{
		if (!(s1[i] == s2[i]))
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

int	ft_isdigit(int d)
{
	if (d >= '0' && d <= '9')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	strg;

	sign = 1;
	strg = 0;
	i = 0;
	ft_isspacemod(&i, str);
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
		{
			sign = sign * -1;
		}
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		strg = (10 * strg) + (str[i] - '0');
		i++;
	}
	return (strg * sign);
}

static int	ft_isspacemod(int *i, const char *strwosp)
{
	while (strwosp[*i] == '\t' || strwosp[*i] == '\n' || strwosp[*i] == '\v'
		|| strwosp[*i] == '\f' || strwosp[*i] == '\r' || strwosp[*i] == ' ')
		(*i)++;
	return (*i);
}

void	free_arr_error_message(char **scene)
{
	free_two_dim_array(scene);
	error_message(1, "identifier syntax");
}
