/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene_pt_nine.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 13:21:43 by aaycan            #+#    #+#             */
/*   Updated: 2025/11/06 13:23:12 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

static void	fill_coordinate_val_x(t_scene *scene, char *coords, int index);
static void	fill_coordinate_val_y(t_scene *scene, char *coords, int index);
static void	fill_coordinate_val_z(t_scene *scene, char *coords, int index);

void	fill_plane_coordinates(t_scene *scene, char *coords, int index)
{
	fill_coordinate_val_x(scene, coords, index);
	while (*coords != ',')
		coords += 1;
	coords += 1;
	fill_coordinate_val_y(scene, coords, index);
	while (*coords != ',')
		coords += 1;
	coords += 1;
	fill_coordinate_val_z(scene, coords, index);
}

static void	fill_coordinate_val_x(t_scene *scene, char *coords, int index)
{
	int	negative;

	negative = 0;
	if (*coords == '-')
	{
		coords++;
		negative = 1;
	}
	scene->plane_data[index].pos_x = (ft_atoi(coords) * 100);
	while ((*coords != '.') && (*coords != ' ') && (*coords != ','))
		coords++;
	if (*coords == '.')
	{
		coords++;
		scene->plane_data[index].pos_x += ((*coords - '0') * 10);
		coords++;
		if ((*coords != ',') && (*coords != ' '))
			scene->plane_data[index].pos_x += (*coords - '0');
	}
	if (negative == 1)
		scene->plane_data[index].pos_x *= -1;
}

static void	fill_coordinate_val_y(t_scene *scene, char *coords, int index)
{
	int	negative;

	negative = 0;
	if (*coords == '-')
	{
		coords++;
		negative = 1;
	}
	scene->plane_data[index].pos_y = (ft_atoi(coords) * 100);
	while ((*coords != '.') && (*coords != ' ') && (*coords != ','))
		coords++;
	if (*coords == '.')
	{
		coords++;
		scene->plane_data[index].pos_y += ((*coords - '0') * 10);
		coords++;
		if ((*coords != ',') && (*coords != ' '))
			scene->plane_data[index].pos_y += (*coords - '0');
	}
	if (negative == 1)
		scene->plane_data[index].pos_y *= -1;
}

static void	fill_coordinate_val_z(t_scene *scene, char *coords, int index)
{
	int	negative;

	negative = 0;
	if (*coords == '-')
	{
		coords++;
		negative = 1;
	}
	scene->plane_data[index].pos_z = (ft_atoi(coords) * 100);
	while ((*coords != '.') && (*coords != ' ') && (*coords != ','))
		coords++;
	if (*coords == '.')
	{
		coords++;
		scene->plane_data[index].pos_z += ((*coords - '0') * 10);
		coords++;
		if ((*coords != ',') && (*coords != ' '))
			scene->plane_data[index].pos_z += (*coords - '0');
	}
	if (negative == 1)
		scene->plane_data[index].pos_z *= -1;
}
