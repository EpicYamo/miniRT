/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene_pt_three.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 23:43:32 by aaycan            #+#    #+#             */
/*   Updated: 2026/02/13 03:08:56 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

static void	fill_plane_data(t_scene *scene, char **scene_map, size_t index,
				size_t i);
static void	fill_cylinder_data(t_scene *scene, char **scene_map, size_t index,
				size_t i);

void	fill_normalized_vector(double *vec_x, double *vec_y, double *vec_z,
	char *vector)
{
	(*vec_x) = ft_atod(vector);
	while (*vector != ',')
		vector += 1;
	vector += 1;
	(*vec_y) = ft_atod(vector);
	while (*vector != ',')
		vector += 1;
	vector += 1;
	(*vec_z) = ft_atod(vector);
}

void	create_plane_data(t_scene *scene, char **scene_map)
{
	size_t	i;
	size_t	index;

	index = -1;
	i = -1;
	while (scene_map[++i])
	{
		if (scene_map[i][0] == 'p')
		{
			index++;
			fill_plane_data(scene, scene_map, index, i);
		}
	}
}

static void	fill_plane_data(t_scene *scene, char **scene_map, size_t index,
	size_t i)
{
	size_t	j;

	j = 2;
	while (scene_map[i][j] == ' ')
		j++;
	fill_coordinates(&scene->plane_data[index].pos_x,
		&scene->plane_data[index].pos_y,
		&scene->plane_data[index].pos_z, &scene_map[i][j]);
	skip_to_next_parameter(scene_map, &i, &j);
	fill_normalized_vector(&scene->plane_data[index].vector_x,
		&scene->plane_data[index].vector_y,
		&scene->plane_data[index].vector_z, &scene_map[i][j]);
	skip_to_next_parameter(scene_map, &i, &j);
	fill_colors(&scene->plane_data[index].red,
		&scene->plane_data[index].green,
		&scene->plane_data[index].blue, &scene_map[i][j]);
}

void	create_cylinder_data(t_scene *scene, char **scene_map)
{
	size_t	i;
	size_t	index;

	index = -1;
	i = -1;
	while (scene_map[++i])
	{
		if (scene_map[i][0] == 'c')
		{
			index++;
			fill_cylinder_data(scene, scene_map, index, i);
		}
	}
}

static void	fill_cylinder_data(t_scene *scene, char **scene_map, size_t index,
	size_t i)
{
	size_t	j;

	j = 2;
	while (scene_map[i][j] == ' ')
		j++;
	fill_coordinates(&scene->cylinder_data[index].pos_x,
		&scene->cylinder_data[index].pos_y,
		&scene->cylinder_data[index].pos_z, &scene_map[i][j]);
	skip_to_next_parameter(scene_map, &i, &j);
	fill_normalized_vector(&scene->cylinder_data[index].vector_x,
		&scene->cylinder_data[index].vector_y,
		&scene->cylinder_data[index].vector_z, &scene_map[i][j]);
	skip_to_next_parameter(scene_map, &i, &j);
	scene->cylinder_data[index].diameter = ft_atod(&scene_map[i][j]);
	skip_to_next_parameter(scene_map, &i, &j);
	scene->cylinder_data[index].height = ft_atod(&scene_map[i][j]);
	skip_to_next_parameter(scene_map, &i, &j);
	fill_colors(&scene->cylinder_data[index].red,
		&scene->cylinder_data[index].green,
		&scene->cylinder_data[index].blue, &scene_map[i][j]);
}
