/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene_pt_two.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 22:58:08 by aaycan            #+#    #+#             */
/*   Updated: 2026/02/13 02:47:47 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	create_ambient_data(t_scene *scene, char **scene_map)
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
			scene->ambient_data.ratio = ft_atod(&scene_map[i][j]);
			skip_to_next_parameter(scene_map, &i, &j);
			fill_colors(&scene->ambient_data.red, &scene->ambient_data.green,
				&scene->ambient_data.blue, &scene_map[i][j]);
		}
	}
}

void	create_camera_data(t_scene *scene, char **scene_map)
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
			fill_coordinates(&scene->camera_data.pos_x,
				&scene->camera_data.pos_y, &scene->camera_data.pos_z,
				&scene_map[i][j]);
			skip_to_next_parameter(scene_map, &i, &j);
			fill_normalized_vector(&scene->camera_data.vector_x,
				&scene->camera_data.vector_y, &scene->camera_data.vector_z,
				&scene_map[i][j]);
			skip_to_next_parameter(scene_map, &i, &j);
			scene->camera_data.fov = ft_atoi(&scene_map[i][j]);
		}
	}
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

void	fill_coordinates(double *pos_x, double *pos_y, double *pos_z,
	char *coords)
{
	(*pos_x) = ft_atod(coords);
	while (*coords != ',')
		coords += 1;
	coords += 1;
	(*pos_y) = ft_atod(coords);
	while (*coords != ',')
		coords += 1;
	coords += 1;
	(*pos_z) = ft_atod(coords);
}
