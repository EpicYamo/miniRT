/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene_pt_three.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 23:35:41 by aaycan            #+#    #+#             */
/*   Updated: 2026/02/12 21:46:48 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	fill_coordinates(double *pos_x, double *pos_y, double *pos_z,
	char *coords)
{
	(*pos_x) = ft_atod(coords);
	while (*coords != ',')
		coords += 1;
	coords += 1;
	(*pos_y) = ft_atod(coords);
	while (*coords != ',')
		coords += 1;
	coords += 1;
	(*pos_z) = ft_atod(coords);
}
