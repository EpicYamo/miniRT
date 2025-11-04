/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene_pt_six.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 13:31:06 by aaycan            #+#    #+#             */
/*   Updated: 2025/11/04 14:38:09 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

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
}
