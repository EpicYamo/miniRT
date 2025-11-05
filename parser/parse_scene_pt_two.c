/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene_pt_two.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 22:58:08 by aaycan            #+#    #+#             */
/*   Updated: 2025/11/05 14:45:54 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

static void	fill_light_ratio(t_scene *scene, char *ratio);

void	fill_ambient_ratio(t_scene *scene, char *ratio)
{
	size_t	i;

	i = 0;
	while ((ratio[i]) && (ratio[i] != ' '))
		i++;
	if (i == 1)
		scene->ambient_data.ratio = (ft_atoi(ratio) * 10);
	else if (i == 3)
		scene->ambient_data.ratio
			= ((ft_atoi(ratio) * 10) + (ft_atoi(ratio + 2)));
}

void	fill_ambient_colors(t_scene *scene, char *range)
{
	scene->ambient_data.red = ft_atoi(range);
	while (*range != ',')
		range += 1;
	range += 1;
	scene->ambient_data.green = ft_atoi(range);
	while (*range != ',')
		range += 1;
	range += 1;
	scene->ambient_data.blue = ft_atoi(range);
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
	scene->light_data.brigthness = 0;
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
			fill_light_coordinates(scene, &scene_map[i][j]);
			while (scene_map[i][j] != ' ')
				j++;
			while (scene_map[i][j] == ' ')
				j++;
			fill_light_ratio(scene, &scene_map[i][j]);
		}
	}
}

static void	fill_light_ratio(t_scene *scene, char *ratio)
{
	size_t	i;

	i = 0;
	while ((ratio[i]) && (ratio[i] != ' '))
		i++;
	if (i == 1)
		scene->light_data.brigthness = (ft_atoi(ratio) * 10);
	else if (i == 3)
		scene->light_data.brigthness
			= ((ft_atoi(ratio) * 10) + (ft_atoi(ratio + 2)));
}
