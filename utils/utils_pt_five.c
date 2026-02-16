/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pt_five.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 20:51:34 by aaycan            #+#    #+#             */
/*   Updated: 2026/02/16 21:01:48 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

int	check_seperator_count(char *str, size_t count)
{
	size_t	i;
	size_t	s_count;

	s_count = 0;
	i = 0;
	if (!(str))
		return (0);
	while (str[i] && (str[i] != ' '))
	{
		if (str[i] == ',')
			s_count++;
		i++;
	}
	if (count == s_count)
		return (1);
	else
		return (0);
}
