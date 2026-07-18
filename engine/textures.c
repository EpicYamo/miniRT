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

void	init_texture(t_rt *rt)
{
	t_scene	*scene;

	scene = rt->old_data->scene;
	scene->tex_loaded = 0;
	scene->tex_img = mlx_xpm_file_to_image(rt->old_data->mlx_ptr,
			(char *)TEXTURE_PATH, &scene->tex_width, &scene->tex_height);
	if (!scene->tex_img)
		return ;
	scene->tex_addr = mlx_get_data_addr(scene->tex_img, &scene->tex_bpp,
			&scene->tex_line, &scene->tex_endian);
	if (!scene->tex_addr)
		return ;
	scene->tex_loaded = 1;
}
