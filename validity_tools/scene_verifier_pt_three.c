/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_verifier_pt_three.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 21:50:33 by aaycan            #+#    #+#             */
/*   Updated: 2025/10/31 15:38:00 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	validate_camera_data(char **scene)
{
	size_t	i;
	size_t	j;

	i = -1;
	while (scene[++i])
	{
		if (scene[i][0] == 'C')
		{
			j = 1;
			while (scene[i][j] == ' ')
				j++;
			//check_camera_coordinates(scene, &scene[i][j]);
			while (scene[i][j] != ' ')
				j++;
			while (scene[i][j] == ' ')
				j++;
			//check_camera_orientation_vector(scene, &scene[i][j]);
			while (scene[i][j] != ' ')
				j++;
			while (scene[i][j] == ' ')
				j++;
			//check_camera_fov(scene, &scene[i][j]);
		}
	}
}
