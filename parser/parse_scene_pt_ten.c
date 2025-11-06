/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene_pt_ten.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 13:23:46 by aaycan            #+#    #+#             */
/*   Updated: 2025/11/06 13:26:07 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

static void	fill_vector_val_x(t_scene *scene, char *vector, int index);
static void	fill_vector_val_y(t_scene *scene, char *vector, int index);
static void	fill_vector_val_z(t_scene *scene, char *vector, int index);

void	fill_plane_vector(t_scene *scene, char *vector, int index)
{
	fill_vector_val_x(scene, vector, index);
	while (*vector != ',')
		vector += 1;
	vector += 1;
	fill_vector_val_y(scene, vector, index);
	while (*vector != ',')
		vector += 1;
	vector += 1;
	fill_vector_val_z(scene, vector, index);
}

static void	fill_vector_val_x(t_scene *scene, char *vector, int index)
{
	int	negative;

	negative = 0;
	if (*vector == '-')
	{
		vector++;
		negative = 1;
	}
	scene->plane_data[index].vector_x = (ft_atoi(vector) * 10);
	while ((*vector != '.') && (*vector != ' ') && (*vector != ','))
		vector++;
	if (*vector == '.')
	{
		vector++;
		scene->plane_data[index].vector_x += ft_atoi(vector);
	}
	if (negative == 1)
		scene->plane_data[index].vector_x *= -1;
}

static void	fill_vector_val_y(t_scene *scene, char *vector, int index)
{
	int	negative;

	negative = 0;
	if (*vector == '-')
	{
		vector++;
		negative = 1;
	}
	scene->plane_data[index].vector_y = (ft_atoi(vector) * 10);
	while ((*vector != '.') && (*vector != ' ') && (*vector != ','))
		vector++;
	if (*vector == '.')
	{
		vector++;
		scene->plane_data[index].vector_y += ft_atoi(vector);
	}
	if (negative == 1)
		scene->plane_data[index].vector_y *= -1;
}

static void	fill_vector_val_z(t_scene *scene, char *vector, int index)
{
	int	negative;

	negative = 0;
	if (*vector == '-')
	{
		vector++;
		negative = 1;
	}
	scene->plane_data[index].vector_z = (ft_atoi(vector) * 10);
	while ((*vector != '.') && (*vector != ' ') && (*vector != ','))
		vector++;
	if (*vector == '.')
	{
		vector++;
		scene->plane_data[index].vector_z += ft_atoi(vector);
	}
	if (negative == 1)
		scene->plane_data[index].vector_z *= -1;
}
