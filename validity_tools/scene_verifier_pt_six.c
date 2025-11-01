/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_verifier_pt_six.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 10:36:51 by aaycan            #+#    #+#             */
/*   Updated: 2025/11/01 14:12:36 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	validate_light_data(char **scene)
{
	size_t	i;
	size_t	j;

	i = -1;
	while (scene[++i])
	{
		if (scene[i][0] == 'L')
		{
			j = 1;
			while (scene[i][j] == ' ')
				j++;
			check_coordinates(scene, &scene[i][j]);
			while (scene[i][j] != ' ')
				j++;
			while (scene[i][j] == ' ')
				j++;
			check_ratio(scene, &scene[i][j]);
		}
	}
}

void	validate_sphere_data(char **scene)
{
	size_t	i;
	size_t	j;

	i = -1;
	while (scene[++i])
	{
		if (scene[i][0] == 's')
		{
			j = 2;
			while (scene[i][j] == ' ')
				j++;
			check_coordinates(scene, &scene[i][j]);
			while (scene[i][j] != ' ')
				j++;
			while (scene[i][j] == ' ')
				j++;
			check_diameter_height(scene, &scene[i][j]);
			while (scene[i][j] != ' ')
				j++;
			while (scene[i][j] == ' ')
				j++;
			check_colors_range(scene, &scene[i][j]);
		}
	}
}

void	check_diameter_height(char **scene, char *value)
{
	size_t	i;
	size_t	dot_count;

	i = 0;
	dot_count = 0;
	while ((value[i]) && (value[i] != ' '))
	{
		if ((value[i] != '.') && (!ft_isdigit(value[i])))
			free_arr_error_message(scene);
		if (value[i] == '.')
			dot_count++;
		i++;
	}
	if (dot_count > 1)
		free_arr_error_message(scene);
}

void	validate_plane_data(char **scene)
{
	size_t	i;
	size_t	j;

	i = -1;
	while (scene[++i])
	{
		if (scene[i][0] == 'p')
		{
			j = 2;
			while (scene[i][j] == ' ')
				j++;
			check_coordinates(scene, &scene[i][j]);
			while (scene[i][j] != ' ')
				j++;
			while (scene[i][j] == ' ')
				j++;
			check_vector_val(scene, &scene[i][j]);
			while (scene[i][j] != ' ')
				j++;
			while (scene[i][j] == ' ')
				j++;
			check_colors_range(scene, &scene[i][j]);
		}
	}
}
