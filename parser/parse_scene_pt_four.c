/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene_pt_four.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 23:43:32 by aaycan            #+#    #+#             */
/*   Updated: 2025/11/03 23:55:55 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

static void	fill_vector_val_x(t_scene *scene, char *vector);
static void	fill_vector_val_y(t_scene *scene, char *vector);
static void	fill_vector_val_z(t_scene *scene, char *vector);

void	fill_camera_orientation_vector(t_scene *scene, char *vector)
{
	fill_vector_val_x(scene, vector);
	while (*vector != ',')
		vector += 1;
	vector += 1;
	fill_vector_val_y(scene, vector);
	while (*vector != ',')
		vector += 1;
	vector += 1;
	fill_vector_val_z(scene, vector);
}

static void	fill_vector_val_x(t_scene *scene, char *vector)
{
	int	negative;

	negative = 0;
	if (*vector == '-')
	{
		vector++;
		negative = 1;
	}
	scene->camera_data.vector_x = (ft_atoi(vector) * 10);
	while ((*vector != '.') && (*vector != ' ') && (*vector != ','))
		vector++;
	if (*vector == '.')
	{
		vector++;
		scene->camera_data.vector_x += ft_atoi(vector);
	}
	if (negative == 1)
		scene->camera_data.vector_x *= -1;
}

static void	fill_vector_val_y(t_scene *scene, char *vector)
{
	int	negative;

	negative = 0;
	if (*vector == '-')
	{
		vector++;
		negative = 1;
	}
	scene->camera_data.vector_y = (ft_atoi(vector) * 10);
	while ((*vector != '.') && (*vector != ' ') && (*vector != ','))
		vector++;
	if (*vector == '.')
	{
		vector++;
		scene->camera_data.vector_y += ft_atoi(vector);
	}
	if (negative == 1)
		scene->camera_data.vector_y *= -1;
}

static void	fill_vector_val_z(t_scene *scene, char *vector)
{
	int	negative;

	negative = 0;
	if (*vector == '-')
	{
		vector++;
		negative = 1;
	}
	scene->camera_data.vector_z = (ft_atoi(vector) * 10);
	while ((*vector != '.') && (*vector != ' ') && (*vector != ','))
		vector++;
	if (*vector == '.')
	{
		vector++;
		scene->camera_data.vector_z += ft_atoi(vector);
	}
	if (negative == 1)
		scene->camera_data.vector_z *= -1;
}
