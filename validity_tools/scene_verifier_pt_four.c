/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_verifier_pt_four.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 16:37:44 by aaycan            #+#    #+#             */
/*   Updated: 2026/02/13 03:11:48 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	check_vector_val(char **scene, char *vector)
{
	size_t	i;
	double	d_ratio;

	if (!(ft_isdigit(vector[0])) && (vector[0] != '-'))
		free_arr_error_message(scene);
	if (vector[0] == '-')
		i = 1;
	else
		i = 0;
	while (ft_isdigit(vector[i]))
		i++;
	if (((vector[i] == ' ' || vector[i] == ','))
		&& ((vector[i - 1] == '0') || (vector[i - 1] == '1')))
		return ;
	if (vector[i++] != '.')
		free_arr_error_message(scene);
	if (!ft_isdigit(vector[i]))
		free_arr_error_message(scene);
	while (ft_isdigit(vector[i]))
		i++;
	if ((vector[i] != ' ') && (vector[i] != ','))
		free_arr_error_message(scene);
	d_ratio = ft_atod(vector);
	if (d_ratio < -1.0 || d_ratio > 1.0)
		free_arr_error_message(scene);
}

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
