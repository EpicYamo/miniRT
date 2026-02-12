/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene_pt_four.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 23:43:32 by aaycan            #+#    #+#             */
/*   Updated: 2026/02/12 21:56:20 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	fill_normalized_vector(double *vec_x, double *vec_y, double *vec_z,
	char *vector)
{
	(*vec_x) = ft_atod(vector);
	while (*vector != ',')
		vector += 1;
	vector += 1;
	(*vec_y) = ft_atod(vector);
	while (*vector != ',')
		vector += 1;
	vector += 1;
	(*vec_z) = ft_atod(vector);
}
