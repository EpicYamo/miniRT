/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene_pt_thirteen.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 13:37:23 by aaycan            #+#    #+#             */
/*   Updated: 2025/11/06 13:45:45 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	fill_cylinder_diameter(t_scene *scene, char *diameter, int index)
{
	scene->cylinder_data[index].diameter = (ft_atoi(diameter) * 100);
	while ((*diameter) && (*diameter != ' ') && (*diameter != '.'))
		diameter++;
	if (*diameter != '.')
		return ;
	diameter++;
	scene->cylinder_data[index].diameter += ((*diameter - '0') * 10);
	diameter++;
	if (ft_isdigit(*diameter))
		scene->cylinder_data[index].diameter += (*diameter - '0');
}

void	fill_cylinder_height(t_scene *scene, char *height, int index)
{
	scene->cylinder_data[index].height = (ft_atoi(height) * 100);
	while ((*height) && (*height != ' ') && (*height != '.'))
		height++;
	if (*height != '.')
		return ;
	height++;
	scene->cylinder_data[index].height += ((*height - '0') * 10);
	height++;
	if (ft_isdigit(*height))
		scene->cylinder_data[index].height += (*height - '0');
}

void	fill_cylinder_colors(t_scene *scene, char *range, int index)
{
	scene->cylinder_data[index].red = ft_atoi(range);
	while (*range != ',')
		range += 1;
	range += 1;
	scene->cylinder_data[index].green = ft_atoi(range);
	while (*range != ',')
		range += 1;
	range += 1;
	scene->cylinder_data[index].blue = ft_atoi(range);
}
