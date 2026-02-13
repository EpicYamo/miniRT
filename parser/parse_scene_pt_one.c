/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene_pt_one.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 14:17:15 by aaycan            #+#    #+#             */
/*   Updated: 2026/02/13 02:45:57 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include <stdlib.h>

static void	fill_blank(t_scene *scene);
static void	create_scene(char **scene_map, t_scene **scene);

t_scene	*parse_scene(char *file_path)
{
	static t_scene	*scene = NULL;
	char			*file;
	char			**scene_map;

	if (!scene)
	{
		file = read_file(file_path);
		scene_map = ft_split(file, '\n');
		free(file);
		if (!scene_map)
			error_message(1, "allocation failed");
		create_scene(scene_map, &scene);
		return (scene);
	}
	else
		return (scene);
}

static void	create_scene(char **scene_map, t_scene **scene)
{
	t_scene	*tmp_scene;

	tmp_scene = malloc(sizeof(t_scene));
	if (!tmp_scene)
	{
		free_two_dim_array(scene_map);
		error_message(1, "allocation failed");
	}
	fill_blank(tmp_scene);
	create_ambient_data(tmp_scene, scene_map);
	create_camera_data(tmp_scene, scene_map);
	create_light_data(tmp_scene, scene_map);
	create_parameter_count(tmp_scene, scene_map);
	create_sphere_data(tmp_scene, scene_map);
	create_plane_data(tmp_scene, scene_map);
	create_cylinder_data(tmp_scene, scene_map);
	(*scene) = tmp_scene;
	free_two_dim_array(scene_map);
}

static void	fill_blank(t_scene *scene)
{
	scene->ambient_data.ratio = 0;
	scene->ambient_data.red = 0;
	scene->ambient_data.green = 0;
	scene->ambient_data.blue = 0;
	scene->ambient_data.existence = 0;
	scene->camera_data.pos_x = 0;
	scene->camera_data.pos_y = 0;
	scene->camera_data.pos_z = 0;
	scene->camera_data.vector_x = 0;
	scene->camera_data.vector_y = 0;
	scene->camera_data.vector_z = 0;
	scene->camera_data.fov = 0;
	scene->camera_data.existence = 0;
	scene->light_data.brightness = 0;
	scene->light_data.pos_x = 0;
	scene->light_data.pos_y = 0;
	scene->light_data.pos_z = 0;
	scene->light_data.existence = 0;
	scene->element_counts.cylinder_count = 0;
	scene->element_counts.plane_count = 0;
	scene->element_counts.sphere_count = 0;
	scene->sphere_data = NULL;
	scene->plane_data = NULL;
	scene->cylinder_data = NULL;
}
