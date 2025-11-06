/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene_pt_one.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 14:17:15 by aaycan            #+#    #+#             */
/*   Updated: 2025/11/06 13:46:40 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include <stdlib.h>

static void	create_scene(char **scene_map, t_scene **scene);
static void	create_ambient_data(t_scene *scene, char **scene_map);
static void	create_camera_data(t_scene *scene, char **scene_map);

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

static void	create_ambient_data(t_scene *scene, char **scene_map)
{
	size_t	i;
	size_t	j;

	i = -1;
	while (scene_map[++i])
	{
		if (scene_map[i][0] == 'A')
		{
			scene->ambient_data.existence = 1;
			j = 1;
			while (scene_map[i][j] == ' ')
				j++;
			fill_ambient_ratio(scene, &scene_map[i][j]);
			while (scene_map[i][j] != ' ')
				j++;
			while (scene_map[i][j] == ' ')
				j++;
			fill_ambient_colors(scene, &scene_map[i][j]);
		}
	}
}

static void	create_camera_data(t_scene *scene, char **scene_map)
{
	size_t	i;
	size_t	j;

	i = -1;
	while (scene_map[++i])
	{
		if (scene_map[i][0] == 'C')
		{
			scene->camera_data.existence = 1;
			j = 1;
			while (scene_map[i][j] == ' ')
				j++;
			fill_camera_coordinates(scene, &scene_map[i][j]);
			while (scene_map[i][j] != ' ')
				j++;
			while (scene_map[i][j] == ' ')
				j++;
			fill_camera_orientation_vector(scene, &scene_map[i][j]);
			while (scene_map[i][j] != ' ')
				j++;
			while (scene_map[i][j] == ' ')
				j++;
			scene->camera_data.fov = ft_atoi(&scene_map[i][j]);
		}
	}
}
