/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_verifier_pt_five.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 15:17:22 by aaycan            #+#    #+#             */
/*   Updated: 2025/11/03 23:53:08 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

static void	check_seperator_count(char **scene, char *coords);
static void	check_coordinate_val(char **scene, char *coords);

void	check_coordinates(char **scene, char *coords)
{
	size_t	i;

	i = 0;
	while ((coords[i]) && (coords[i] != ' '))
		i++;
	if (i < 5)
		free_arr_error_message(scene);
	check_seperator_count(scene, coords);
	check_coordinate_val(scene, coords);
	while (*coords != ',')
		coords += 1;
	coords += 1;
	check_coordinate_val(scene, coords);
	while (*coords != ',')
		coords += 1;
	coords += 1;
	check_coordinate_val(scene, coords);
}

static void	check_seperator_count(char **scene, char *coords)
{
	size_t	i;
	size_t	seperator_count;

	i = 0;
	seperator_count = 0;
	while ((coords[i]) && (coords[i] != ' '))
	{
		if (coords[i] == ',')
			seperator_count++;
		if ((coords[i] != '-') && (coords[i] != '.') && (coords[i] != ',')
			&& (!ft_isdigit(coords[i])))
			free_arr_error_message(scene);
		i++;
	}
	if (seperator_count != 2)
		free_arr_error_message(scene);
}

static void	check_coordinate_val(char **scene, char *coords)
{
	size_t	i;
	size_t	dot_count;

	if ((coords[0] == ',') || (coords[0] == '.'))
		free_arr_error_message(scene);
	i = 0;
	dot_count = 0;
	while ((coords[i]) && (coords[i] != ',') && (coords[i] != ' '))
	{
		if (coords[i] == '.')
			dot_count += 1;
		i++;
	}
	if (dot_count > 1)
		free_arr_error_message(scene);
	if (dot_count == 1)
	{
		while (*coords != '.')
			coords++;
		coords++;
		if (!ft_isdigit(*coords))
			free_arr_error_message(scene);
	}
}
