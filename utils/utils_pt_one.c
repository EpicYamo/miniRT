/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pt_one.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 16:56:13 by aaycan            #+#    #+#             */
/*   Updated: 2025/10/27 23:06:50 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	error_message(int code, char *str)
{
	printf("\033[0;31mError: %s\033[0m\n", str);
	exit(code);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	int		i;
	int		j;
	size_t	total_len;

	if (!s1 || !s2)
		return (NULL);
	i = 0;
	total_len = ft_strlen(s1) + ft_strlen(s2);
	result = (char *)malloc((total_len * sizeof(char)) + 1);
	if (result == NULL)
		return (NULL);
	while (s1[i] != '\0')
	{
		result[i] = ((unsigned char *)s1)[i];
		i++;
	}
	j = 0;
	while (s2[j] != '\0')
		result[i++] = ((unsigned char *)s2)[j++];
	result[i] = '\0';
	return (result);
}
