/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene_pt_six.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 13:31:06 by aaycan            #+#    #+#             */
/*   Updated: 2025/11/05 15:07:00 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include <stdlib.h>

static void	allocate_sphere_space(t_scene *scene);
static void	allocate_plane_space(t_scene *scene);
static void	allocate_cylinder_space(t_scene *scene);

void	create_parameter_count(t_scene *scene, char **scene_map)
{
	size_t	i;

	i = -1;
	while (scene_map[++i])
	{
		if (scene_map[i][0] == 's')
			scene->element_counts.sphere_count += 1;
		else if (scene_map[i][0] == 'p')
			scene->element_counts.plane_count += 1;
		else if (scene_map[i][0] == 'c')
			scene->element_counts.cylinder_count += 1;
	}
	if (scene->element_counts.sphere_count > 0)
		allocate_sphere_space(scene);
	if (scene->element_counts.plane_count > 0)
		allocate_plane_space(scene);
	if (scene->element_counts.cylinder_count > 0)
		allocate_cylinder_space(scene);
}

static void	allocate_sphere_space(t_scene *scene)
{
	scene->sphere_data = malloc(sizeof(t_sphere_data) * scene->element_counts.sphere_count);
	if (!(scene->sphere_data))
	{
		free(scene);
		error_message(1, "allocation failed");
	}
}

static void	allocate_plane_space(t_scene *scene)
{
	scene->plane_data = malloc(sizeof(t_plane_data) * scene->element_counts.plane_count);
	if (!(scene->plane_data))
	{
		free(scene->sphere_data);
		free(scene);
		error_message(1, "allocation failed");
	}
}

static void	allocate_cylinder_space(t_scene *scene)
{
	scene->cylinder_data = malloc(sizeof(t_cylinder_data) * scene->element_counts.cylinder_count);
	if (!(scene->cylinder_data))
	{
		free(scene->sphere_data);
		free(scene->plane_data);
		free(scene);
		error_message(1, "allocation failed");
	}
}

void	create_sphere_data(t_scene *scene, char **scene_map)
{
	size_t	i;
	size_t	j;
	size_t	index;

	index = -1;
	i = -1;
	while (scene_map[++i])
	{
		if (scene_map[i][0] == 's')
		{
			index++;
			j = 2;
			while (scene_map[i][j] == ' ')
				j++;
			fill_sphere_coordinates(scene, &scene_map[i][j], index);
			increment_index_to_the_next_param(scene_map[i], &j);
			fill_sphere_diameter(scene, &scene_map[i][j], index);
			increment_index_to_the_next_param(scene_map[i], &j);
			fill_sphere_colors(scene, &scene_map[i][j], index);
		}
	}
}
