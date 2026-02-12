/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_verifier_pt_seven.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 10:46:45 by aaycan            #+#    #+#             */
/*   Updated: 2026/02/12 22:12:29 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	validate_cylinder_data(char **scene)
{
	size_t	i;
	size_t	j;

	i = -1;
	while (scene[++i])
	{
		if (scene[i][0] == 'c')
		{
			j = 2;
			while (scene[i][j] == ' ')
				j++;
			check_coordinates(scene, &scene[i][j]);
			skip_to_next_parameter(scene, &i, &j);
			check_vector_val(scene, &scene[i][j]);
			skip_to_next_parameter(scene, &i, &j);
			check_diameter_height(scene, &scene[i][j]);
			skip_to_next_parameter(scene, &i, &j);
			check_diameter_height(scene, &scene[i][j]);
			skip_to_next_parameter(scene, &i, &j);
			check_colors_range(scene, &scene[i][j]);
		}
	}
}
