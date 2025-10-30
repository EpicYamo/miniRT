/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_verifier_pt_two.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 21:32:09 by aaycan            #+#    #+#             */
/*   Updated: 2025/10/30 22:09:03 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include <stdlib.h>

static void	check_ambient_ratio(char **scene, char *ratio);
static void	check_ambient_colors_range(char **scene, char *range);

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
	{
		free_two_dim_array(scene);
		error_message(1, "identifier syntax");
	}
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
			check_ambient_ratio(scene, &scene[i][j]);
			j += 3;
			while (scene[i][j] == ' ')
				j++;
			check_ambient_colors_range(scene, &scene[i][j]);
		}
	}
}

static void	check_ambient_ratio(char **scene, char *ratio)
{
	if (ratio[3] != ' ')
	{
		free_two_dim_array(scene);
		error_message(1, "identifier syntax");
	}
	if ((ratio[0] != '0' && ratio[0] != '1') || (ratio[1] != '.'))
	{
		free_two_dim_array(scene);
		error_message(1, "identifier syntax");
	}
	if ((ratio[0] == '1') && (ratio[2] != '0'))
	{
		free_two_dim_array(scene);
		error_message(1, "identifier syntax");
	}
	if (!ft_isdigit(ratio[2]))
	{
		free_two_dim_array(scene);
		error_message(1, "identifier syntax");
	}
}

static void	check_ambient_colors_range(char **scene, char *range)
{
	
}
