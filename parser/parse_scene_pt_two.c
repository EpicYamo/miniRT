/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene_pt_two.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 22:58:08 by aaycan            #+#    #+#             */
/*   Updated: 2026/02/12 22:24:48 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	fill_colors(unsigned int *red, unsigned int *green,
	unsigned int *blue, char *range)
{
	(*red) = ft_atoi(range);
	while (*range != ',')
		range += 1;
	range += 1;
	(*green) = ft_atoi(range);
	while (*range != ',')
		range += 1;
	range += 1;
	(*blue) = ft_atoi(range);
}

void	fill_blank(t_scene *scene)
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

void	create_light_data(t_scene *scene, char **scene_map)
{
	size_t	i;
	size_t	j;

	i = -1;
	while (scene_map[++i])
	{
		if (scene_map[i][0] == 'L')
		{
			scene->light_data.existence = 1;
			j = 1;
			while (scene_map[i][j] == ' ')
				j++;
			fill_coordinates(&scene->light_data.pos_x,
				&scene->light_data.pos_y, &scene->light_data.pos_z,
				&scene_map[i][j]);
			skip_to_next_parameter(scene_map, &i, &j);
			scene->light_data.brightness = ft_atod(&scene_map[i][j]);
		}
	}
}
