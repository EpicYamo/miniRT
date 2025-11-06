/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pt_four.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 14:03:46 by aaycan            #+#    #+#             */
/*   Updated: 2025/11/06 13:49:50 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include <stdlib.h>

void	increment_index_to_the_next_param(char *scene_map, size_t *j)
{
	while (scene_map[*j] != ' ')
		(*j)++;
	while (scene_map[*j] == ' ')
		(*j)++;
}

void	free_scene(char *file_path)
{
	t_scene	*scene;

	scene = parse_scene(file_path);
	free(scene->cylinder_data);
	free(scene->plane_data);
	free(scene->sphere_data);
	free(scene);
}
