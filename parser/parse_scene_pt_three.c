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
static void	fill_cube_data(t_scene *scene, char **scene_map, size_t index,
				size_t i);
static void	fill_triangle_data(t_scene *scene, char **scene_map,
				size_t index, size_t i);

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
	scene->plane_data[index].checker = 0;
	scene->plane_data[index].shininess = DEFAULT_SHININESS;
	scene->plane_data[index].specular_strength = DEFAULT_SPEC_STRENGTH;
	scene->plane_data[index].texture_id = -1;
	scene->plane_data[index].tex_repeat = 1.0;
	scene->plane_data[index].bump_strength = 0.0;
}

void	create_cylinder_data(t_scene *scene, char **scene_map)
{
	size_t	i;
	size_t	index;

	index = -1;
	i = -1;
	while (scene_map[++i])
	{
		if (scene_map[i][0] == 'c' && scene_map[i][1] == 'y')
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
	scene->cylinder_data[index].checker = 0;
	scene->cylinder_data[index].shininess = DEFAULT_SHININESS;
	scene->cylinder_data[index].specular_strength = DEFAULT_SPEC_STRENGTH;
	scene->cylinder_data[index].texture_id = -1;
	scene->cylinder_data[index].tex_repeat = 1.0;
	scene->cylinder_data[index].bump_strength = 0.0;
}

void	create_cube_data(t_scene *scene, char **scene_map)
{
	size_t	i;
	size_t	index;

	index = -1;
	i = -1;
	while (scene_map[++i])
	{
		if (scene_map[i][0] == 'c' && scene_map[i][1] == 'u')
		{
			index++;
			fill_cube_data(scene, scene_map, index, i);
		}
	}
}

static void	fill_cube_data(t_scene *scene, char **scene_map, size_t index,
	size_t i)
{
	size_t	j;

	j = 2;
	while (scene_map[i][j] == ' ')
		j++;
	fill_coordinates(&scene->cube_data[index].pos_x,
		&scene->cube_data[index].pos_y,
		&scene->cube_data[index].pos_z, &scene_map[i][j]);
	skip_to_next_parameter(scene_map, &i, &j);
	fill_normalized_vector(&scene->cube_data[index].vector_x,
		&scene->cube_data[index].vector_y,
		&scene->cube_data[index].vector_z, &scene_map[i][j]);
	skip_to_next_parameter(scene_map, &i, &j);
	scene->cube_data[index].size = ft_atod(&scene_map[i][j]);
	skip_to_next_parameter(scene_map, &i, &j);
	fill_colors(&scene->cube_data[index].red,
		&scene->cube_data[index].green,
		&scene->cube_data[index].blue, &scene_map[i][j]);
	scene->cube_data[index].id = 0;
	scene->cube_data[index].checker = 0;
	scene->cube_data[index].shininess = DEFAULT_SHININESS;
	scene->cube_data[index].specular_strength = DEFAULT_SPEC_STRENGTH;
	scene->cube_data[index].texture_id = -1;
	scene->cube_data[index].tex_repeat = 1.0;
	scene->cube_data[index].bump_strength = 0.0;
}

void	create_triangle_data(t_scene *scene, char **scene_map)
{
	size_t	i;
	size_t	index;

	index = -1;
	i = -1;
	while (scene_map[++i])
	{
		if (scene_map[i][0] == 't' && scene_map[i][1] == 'r')
		{
			index++;
			fill_triangle_data(scene, scene_map, index, i);
		}
	}
}

static void	fill_triangle_data(t_scene *scene, char **scene_map, size_t index,
	size_t i)
{
	size_t	j;

	j = 2;
	while (scene_map[i][j] == ' ')
		j++;
	fill_coordinates(&scene->triangle_data[index].pos_x,
		&scene->triangle_data[index].pos_y,
		&scene->triangle_data[index].pos_z, &scene_map[i][j]);
	skip_to_next_parameter(scene_map, &i, &j);
	fill_normalized_vector(&scene->triangle_data[index].vector_x,
		&scene->triangle_data[index].vector_y,
		&scene->triangle_data[index].vector_z, &scene_map[i][j]);
	skip_to_next_parameter(scene_map, &i, &j);
	scene->triangle_data[index].size = ft_atod(&scene_map[i][j]);
	skip_to_next_parameter(scene_map, &i, &j);
	scene->triangle_data[index].depth = ft_atod(&scene_map[i][j]);
	skip_to_next_parameter(scene_map, &i, &j);
	scene->triangle_data[index].kind = ft_atoi(&scene_map[i][j]);
	skip_to_next_parameter(scene_map, &i, &j);
	fill_colors(&scene->triangle_data[index].red,
		&scene->triangle_data[index].green,
		&scene->triangle_data[index].blue, &scene_map[i][j]);
	scene->triangle_data[index].id = 0;
	scene->triangle_data[index].checker = 0;
	scene->triangle_data[index].shininess = DEFAULT_SHININESS;
	scene->triangle_data[index].specular_strength = DEFAULT_SPEC_STRENGTH;
	scene->triangle_data[index].texture_id = -1;
	scene->triangle_data[index].tex_repeat = 1.0;
	scene->triangle_data[index].bump_strength = 0.0;
}
