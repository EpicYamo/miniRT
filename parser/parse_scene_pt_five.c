/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene_pt_five.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 13:20:01 by aaycan            #+#    #+#             */
/*   Updated: 2025/11/04 13:30:28 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

static void	fill_coordinate_val_x(t_scene *scene, char *coords);
static void	fill_coordinate_val_y(t_scene *scene, char *coords);
static void	fill_coordinate_val_z(t_scene *scene, char *coords);

void	fill_light_coordinates(t_scene *scene, char *coords)
{
	fill_coordinate_val_x(scene, coords);
	while (*coords != ',')
		coords += 1;
	coords += 1;
	fill_coordinate_val_y(scene, coords);
	while (*coords != ',')
		coords += 1;
	coords += 1;
	fill_coordinate_val_z(scene, coords);
}

static void	fill_coordinate_val_x(t_scene *scene, char *coords)
{
	int	negative;

	negative = 0;
	if (*coords == '-')
	{
		coords++;
		negative = 1;
	}
	scene->light_data.pos_x = (ft_atoi(coords) * 100);
	while ((*coords != '.') && (*coords != ' ') && (*coords != ','))
		coords++;
	if (*coords == '.')
	{
		coords++;
		scene->light_data.pos_x += ((*coords - '0') * 10);
		coords++;
		if ((*coords != ',') && (*coords != ' '))
			scene->light_data.pos_x += (*coords - '0');
	}
	if (negative == 1)
		scene->light_data.pos_x *= -1;
}

static void	fill_coordinate_val_y(t_scene *scene, char *coords)
{
	int	negative;

	negative = 0;
	if (*coords == '-')
	{
		coords++;
		negative = 1;
	}
	scene->light_data.pos_y = (ft_atoi(coords) * 100);
	while ((*coords != '.') && (*coords != ' ') && (*coords != ','))
		coords++;
	if (*coords == '.')
	{
		coords++;
		scene->light_data.pos_y += ((*coords - '0') * 10);
		coords++;
		if ((*coords != ',') && (*coords != ' '))
			scene->light_data.pos_y += (*coords - '0');
	}
	if (negative == 1)
		scene->light_data.pos_y *= -1;
}

static void	fill_coordinate_val_z(t_scene *scene, char *coords)
{
	int	negative;

	negative = 0;
	if (*coords == '-')
	{
		coords++;
		negative = 1;
	}
	scene->light_data.pos_z = (ft_atoi(coords) * 100);
	while ((*coords != '.') && (*coords != ' ') && (*coords != ','))
		coords++;
	if (*coords == '.')
	{
		coords++;
		scene->light_data.pos_z += ((*coords - '0') * 10);
		coords++;
		if ((*coords != ',') && (*coords != ' '))
			scene->light_data.pos_z += (*coords - '0');
	}
	if (negative == 1)
		scene->light_data.pos_z *= -1;
}
