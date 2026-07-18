/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include "../minilibx-linux/mlx.h"
#include <stdlib.h>
#include <string.h>

static int	find_texture(t_scene *scene, char *name)
{
	int	i;

	i = 0;
	while (i < scene->texture_count)
	{
		if (ft_strncmp(scene->textures[i].name, name, 64) == 0)
			return (i);
		i++;
	}
	return (-1);
}

static int	load_new_texture(t_rt *rt, char *name)
{
	t_scene		*scene;
	t_texture	*new_arr;
	char		path[128];
	t_texture	tex;

	scene = rt->old_data->scene;
	path[0] = '\0';
	strcat(path, TEXTURE_DIR);
	strcat(path, name);
	tex.img = mlx_xpm_file_to_image(rt->old_data->mlx_ptr, path,
			&tex.width, &tex.height);
	if (!tex.img)
		return (-1);
	tex.addr = mlx_get_data_addr(tex.img, &tex.bpp, &tex.line, &tex.endian);
	if (!tex.addr)
	{
		mlx_destroy_image(rt->old_data->mlx_ptr, tex.img);
		return (-1);
	}
	new_arr = realloc(scene->textures,
			sizeof(t_texture) * (scene->texture_count + 1));
	if (!new_arr)
	{
		mlx_destroy_image(rt->old_data->mlx_ptr, tex.img);
		return (-1);
	}
	scene->textures = new_arr;
	strncpy(scene->textures[scene->texture_count].name, name, 63);
	scene->textures[scene->texture_count].name[63] = '\0';
	scene->textures[scene->texture_count].img = tex.img;
	scene->textures[scene->texture_count].addr = tex.addr;
	scene->textures[scene->texture_count].width = tex.width;
	scene->textures[scene->texture_count].height = tex.height;
	scene->textures[scene->texture_count].bpp = tex.bpp;
	scene->textures[scene->texture_count].line = tex.line;
	scene->textures[scene->texture_count].endian = tex.endian;
	scene->texture_count++;
	return (scene->texture_count - 1);
}

int	get_or_load_texture(t_rt *rt, char *name)
{
	int	existing;

	if (!name || name[0] == '\0')
		return (-1);
	existing = find_texture(rt->old_data->scene, name);
	if (existing != -1)
		return (existing);
	return (load_new_texture(rt, name));
}
