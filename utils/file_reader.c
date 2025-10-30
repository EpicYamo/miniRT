/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_reader.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 19:11:27 by aaycan            #+#    #+#             */
/*   Updated: 2025/10/30 17:24:40 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static void	read_file_func(int fd, char **scene);
static void	first_allocation(char **scene, char *tmp, int fd);
static void	close_error(int fd, int option);

char	*read_file(char *file_path)
{
	int		fd;
	char	*scene;

	scene = NULL;
	fd = open(file_path, O_RDONLY);
	if (fd == -1)
	{
		perror("Error");
		exit (1);
	}
	read_file_func(fd, &scene);
	return (scene);
}

static void	read_file_func(int fd, char **scene)
{
	char	tmp[43];
	char	*join_tmp;
	ssize_t	read_bytes;

	while (1)
	{
		read_bytes = read(fd, tmp, 42);
		tmp[read_bytes] = '\0';
		if (read_bytes == 0)
			break ;
		if (read_bytes == -1)
			close_error(fd, 1);
		if ((*scene) == NULL)
			first_allocation(scene, tmp, fd);
		else
		{
			join_tmp = ft_strjoin(*scene, tmp);
			free(*scene);
			if (join_tmp == NULL)
				close_error(fd, 2);
			(*scene) = join_tmp;
		}
	}
	close(fd);
}

static void	first_allocation(char **scene, char *tmp, int fd)
{
	size_t	i;

	if ((*scene) == NULL)
	{
		(*scene) = malloc(sizeof(char) * 43);
		if ((*scene) == NULL)
		{
			close(fd);
			error_message(1, "memory allocation failed");
		}
	}
	i = -1;
	while (tmp[++i])
		(*scene)[i] = tmp[i];
	(*scene)[i] = '\0';
}

static void	close_error(int fd, int option)
{
	close(fd);
	if (option == 1)
		error_message(1, "read function");
	error_message(1, "memory allocation failed");
}
