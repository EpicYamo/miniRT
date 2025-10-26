/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_file_validity.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 16:44:20 by aaycan            #+#    #+#             */
/*   Updated: 2025/10/26 18:37:34 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

static void	check_empty_file(char *file_path);

int	check_file(char *file_path)
{
	size_t	i;

	i = ft_strlen(file_path);
	if (i <= 3)
		return (1);
	if ((file_path[i - 1] != 't') || (file_path[i - 2] != 'r')
		|| (file_path[i - 3] != '.'))
		return (1);
	check_empty_file(file_path);
	return (0);
}

static void	check_empty_file(char *file_path)
{
	int		fd;
	char	buffer[10];

	fd = open(file_path, O_RDONLY);
	if (fd == -1)
	{
		perror("Error");
		exit (1);
	}
	if (read(fd, buffer, 10) <= 0)
	{
		close(fd);
		error_message(1, "file is empty or inaccessible");
	}
	close(fd);
}
