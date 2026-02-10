/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_verifier_pt_two.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 21:32:09 by aaycan            #+#    #+#             */
/*   Updated: 2026/02/11 01:22:31 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

static void	check_range_values(char **scene, char *range);

void	check_param_count(char **scene, char *params, int params_c)
{
	int	fail;

	fail = 1;
	if (params_c == 3)
	{
		if ((params[0] == 'A') || (params[0] == 'L'))
			fail = 0;
	}
	else if (params_c == 4)
	{
		if ((params[0] == 'C') || (!ft_strncmp(params, "sp", 2))
			|| (!ft_strncmp(params, "pl", 2)))
			fail = 0;
	}
	else if (params_c == 6)
	{
		if (!ft_strncmp(params, "cy", 2))
			fail = 0;
	}
	if (fail != 0)
		free_arr_error_message(scene);
}

void	validate_ambient_data(char **scene)
{
	size_t	i;
	size_t	j;

	i = -1;
	while (scene[++i])
	{
		if (scene[i][0] == 'A')
		{
			j = 1;
			while (scene[i][j] == ' ')
				j++;
			check_ratio(scene, &scene[i][j]);
			while (scene[i][j] != ' ')
				j++;
			while (scene[i][j] == ' ')
				j++;
			check_colors_range(scene, &scene[i][j]);
		}
	}
}

void	check_ratio(char **scene, char *ratio)
{
	size_t	i;
	double	d_ratio;

	if (!(ft_isdigit(ratio[0])))
		free_arr_error_message(scene);
	i = 0;
	while (ft_isdigit(ratio[i]))
		i++;
	if ((i == 1) && (ratio[i] == ' ') && ((ratio[0] == '0') || (ratio[0] == '1')))
		return ;
	if (ratio[i] != '.')
		free_arr_error_message(scene);
	i++;
	if (!ft_isdigit(ratio[i]))
		free_arr_error_message(scene);
	while (ft_isdigit(ratio[i]))
		i++;
	if ((ratio[i] != ' ') && (ratio[i] != '\0'))
		free_arr_error_message(scene);
	d_ratio = ft_atod(ratio);
	if (d_ratio < 0.0 || d_ratio > 1.0)
		free_arr_error_message(scene);
}

void	check_colors_range(char **scene, char *range)
{
	size_t	i;
	size_t	seperator_count;
	size_t	range_size;

	if (ft_strlen(range) < 5)
		free_arr_error_message(scene);
	if (!ft_isdigit(range[0]))
		free_arr_error_message(scene);
	i = -1;
	seperator_count = 0;
	range_size = 0;
	while ((range[i + 1]) && (range[++i] != ' '))
	{
		if (range[i] == ',')
			seperator_count++;
		if ((range[i] != ',') && (!ft_isdigit(range[i])))
			free_arr_error_message(scene);
		range_size++;
	}
	if ((seperator_count != 2) || (range_size > 11))
		free_arr_error_message(scene);
	check_range_values(scene, range);
}

static void	check_range_values(char **scene, char *range)
{
	if ((ft_atoi(range) < 0) || (ft_atoi(range) > 255))
		free_arr_error_message(scene);
	while (*range != ',')
		range += 1;
	range += 1;
	if ((ft_atoi(range) < 0) || (ft_atoi(range) > 255))
		free_arr_error_message(scene);
	while (*range != ',')
		range += 1;
	range += 1;
	if ((ft_atoi(range) < 0) || (ft_atoi(range) > 255))
		free_arr_error_message(scene);
}
