/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene_pt_eight.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 14:46:20 by aaycan            #+#    #+#             */
/*   Updated: 2025/11/06 13:45:36 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

static void	fill_plane_colors(t_scene *scene, char *range, int index);

void	fill_sphere_diameter(t_scene *scene, char *diameter, int index)
{
	scene->sphere_data[index].diameter = (ft_atoi(diameter) * 100);
	while ((*diameter) && (*diameter != ' ') && (*diameter != '.'))
		diameter++;
	if (*diameter != '.')
		return ;
	diameter++;
	scene->sphere_data[index].diameter += ((*diameter - '0') * 10);
	diameter++;
	if (ft_isdigit(*diameter))
		scene->sphere_data[index].diameter += (*diameter - '0');
}

void	fill_sphere_colors(t_scene *scene, char *range, int index)
{
	scene->sphere_data[index].red = ft_atoi(range);
	while (*range != ',')
		range += 1;
	range += 1;
	scene->sphere_data[index].green = ft_atoi(range);
	while (*range != ',')
		range += 1;
	range += 1;
	scene->sphere_data[index].blue = ft_atoi(range);
}

void	create_plane_data(t_scene *scene, char **scene_map)
{
	size_t	i;
	size_t	j;
	size_t	index;

	index = -1;
	i = -1;
	while (scene_map[++i])
	{
		if (scene_map[i][0] == 'p')
		{
			index++;
			j = 2;
			while (scene_map[i][j] == ' ')
				j++;
			fill_plane_coordinates(scene, &scene_map[i][j], index);
			increment_index_to_the_next_param(scene_map[i], &j);
			fill_plane_vector(scene, &scene_map[i][j], index);
			increment_index_to_the_next_param(scene_map[i], &j);
			fill_plane_colors(scene, &scene_map[i][j], index);
		}
	}
}

static void	fill_plane_colors(t_scene *scene, char *range, int index)
{
	scene->plane_data[index].red = ft_atoi(range);
	while (*range != ',')
		range += 1;
	range += 1;
	scene->plane_data[index].green = ft_atoi(range);
	while (*range != ',')
		range += 1;
	range += 1;
	scene->plane_data[index].blue = ft_atoi(range);
}

void	create_cylinder_data(t_scene *scene, char **scene_map)
{
	size_t	i;
	size_t	j;
	size_t	index;

	index = -1;
	i = -1;
	while (scene_map[++i])
	{
		if (scene_map[i][0] == 'c')
		{
			index++;
			j = 2;
			while (scene_map[i][j] == ' ')
				j++;
			fill_cylinder_coordinates(scene, &scene_map[i][j], index);
			increment_index_to_the_next_param(scene_map[i], &j);
			fill_cylinder_vector(scene, &scene_map[i][j], index);
			increment_index_to_the_next_param(scene_map[i], &j);
			fill_cylinder_diameter(scene, &scene_map[i][j], index);
			increment_index_to_the_next_param(scene_map[i], &j);
			fill_cylinder_height(scene, &scene_map[i][j], index);
			increment_index_to_the_next_param(scene_map[i], &j);
			fill_cylinder_colors(scene, &scene_map[i][j], index);
		}
	}
}
