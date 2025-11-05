/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene_pt_eight.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 14:46:20 by aaycan            #+#    #+#             */
/*   Updated: 2025/11/05 15:03:17 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	fill_sphere_diameter(t_scene *scene, char *diameter, int index)
{
	scene->sphere_data[index].diameter = (ft_atoi(diameter) * 100);
	while ((*diameter) && (*diameter != ' ') && (*diameter != '.'))
		diameter++;
	if (*diameter != '.')
		return ;
	diameter++;
	scene->sphere_data[index].diameter += ((*diameter - '0') * 10);
	diameter++;
	if (ft_isdigit(*diameter))
		scene->sphere_data[index].diameter += (*diameter - '0');
}

void	fill_sphere_colors(t_scene *scene, char *range, int index)
{
	scene->sphere_data[index].red = ft_atoi(range);
	while (*range != ',')
		range += 1;
	range += 1;
	scene->sphere_data[index].green = ft_atoi(range);
	while (*range != ',')
		range += 1;
	range += 1;
	scene->sphere_data[index].blue = ft_atoi(range);
}
