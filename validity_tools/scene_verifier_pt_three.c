/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_verifier_pt_three.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 21:50:33 by aaycan            #+#    #+#             */
/*   Updated: 2026/02/12 17:32:53 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

static void	check_camera_fov(char **scene, char *fov);

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
			check_coordinates(scene, &scene[i][j]);
			while (scene[i][j] != ' ')
				j++;
			while (scene[i][j] == ' ')
				j++;
			check_normalized_vector(scene, &scene[i][j]);
			while (scene[i][j] != ' ')
				j++;
			while (scene[i][j] == ' ')
				j++;
			check_camera_fov(scene, &scene[i][j]);
		}
	}
}

void	check_normalized_vector(char **scene, char *vector)
{
	size_t	i;
	size_t	seperator_count;

	i = 0;
	seperator_count = 0;
	while ((vector[i]) && (vector[i] != ' '))
	{
		if ((vector[i] != '.') && (vector[i] != ',') && (vector[i] != '-')
			&& (!ft_isdigit(vector[i])))
			free_arr_error_message(scene);
		if (vector[i] == ',')
			seperator_count++;
		i++;
	}
	if ((seperator_count != 2) || (i < 5))
		free_arr_error_message(scene);
	check_vector_val(scene, vector);
	while (*vector != ',')
		vector += 1;
	vector += 1;
	check_vector_val(scene, vector);
	while (*vector != ',')
		vector += 1;
	vector += 1;
	check_vector_val(scene, vector);
}

static void	check_camera_fov(char **scene, char *fov)
{
	size_t	i;

	i = 0;
	while ((fov[i]) && (fov[i] != ' '))
	{
		if (!ft_isdigit(fov[i]))
			free_arr_error_message(scene);
		i++;
	}
	if (i > 3)
		free_arr_error_message(scene);
	if ((ft_atoi(fov) < 0) || (ft_atoi(fov) > 180))
		free_arr_error_message(scene);
}
