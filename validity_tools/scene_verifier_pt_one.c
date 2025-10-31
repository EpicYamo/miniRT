/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_verifier_pt_one.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 23:36:50 by aaycan            #+#    #+#             */
/*   Updated: 2025/10/31 15:36:05 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include <stdlib.h>

static void	validate_identifier_names(char **scene);
static void	validate_identifier_count(char **scene);
static void	validate_parameter_count(char **scene);

void	check_scene(char *file_path)
{
	char	*file;
	char	**scene;

	file = read_file(file_path);
	scene = ft_split(file, '\n');
	free(file);
	if (!scene)
		error_message(1, "allocation failed");
	validate_identifier_names(scene);
	validate_identifier_count(scene);
	validate_parameter_count(scene);
	validate_ambient_data(scene);
	validate_camera_data(scene);
	//validate_light_data(scene);
	//validate_sphere_data(scene);
	//validate_plane_data(scene);
	//validate_cylinder_data(scene);
	free_two_dim_array(scene);
}

static void	validate_identifier_names(char **scene)
{
	size_t	i;

	i = -1;
	while (scene[++i])
	{
		if ((ft_strncmp(scene[i], "A ", 2) != 0)
			&& (ft_strncmp(scene[i], "C ", 2) != 0)
			&& (ft_strncmp(scene[i], "L ", 2) != 0)
			&& (ft_strncmp(scene[i], "sp ", 3) != 0)
			&& (ft_strncmp(scene[i], "pl ", 3) != 0)
			&& (ft_strncmp(scene[i], "cy ", 3) != 0))
			free_arr_error_message(scene);
	}
}

static void	validate_identifier_count(char **scene)
{
	size_t	i;
	size_t	cam_count;
	size_t	light_count;
	size_t	ambient_count;

	cam_count = 0;
	light_count = 0;
	ambient_count = 0;
	i = -1;
	while (scene[++i])
	{
		if (ft_strncmp(scene[i], "A ", 2) == 0)
			ambient_count++;
		else if (ft_strncmp(scene[i], "L ", 2) == 0)
			light_count++;
		else if (ft_strncmp(scene[i], "C ", 2) == 0)
			cam_count++;
		if ((cam_count > 1) || (light_count > 1) || (ambient_count > 1))
			free_arr_error_message(scene);
	}
}

static void	validate_parameter_count(char **scene)
{
	size_t	i;
	size_t	j;
	size_t	param_count;
	int		flag;

	i = -1;
	while (scene[++i])
	{
		flag = 0;
		j = -1;
		param_count = 0;
		while (scene[i][++j])
		{
			if (scene[i][j] == ' ')
				flag = 0;
			if ((flag != 1) && (scene[i][j] != ' '))
			{
				flag = 1;
				param_count++;
			}
		}
		check_param_count(scene, scene[i], param_count);
	}
}
