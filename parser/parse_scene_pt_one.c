/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene_pt_one.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 14:17:15 by aaycan            #+#    #+#             */
/*   Updated: 2025/11/01 14:50:19 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include <stdlib.h>

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
	free_two_dim_array(scene_map);
	if (!tmp_scene)
		error_message(1, "allocation failed");
	//create_ambient_data(tmp_scene, g_c);
	//create_camera_data(tmp_scene, g_c);
	//create_light_data(tmp_scene, g_c);
	//create_parameter_count(tmp_scene, g_c);
	//create_sphere_data(tmp_scene, g_c);
	//create_plane_data(tmp_scene, g_c);
	//create_cylinder_data(tmp_scene, g_c);
	(*scene) = tmp_scene;
}
