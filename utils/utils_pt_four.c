/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pt_four.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 14:03:46 by aaycan            #+#    #+#             */
/*   Updated: 2025/11/05 15:05:26 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	increment_index_to_the_next_param(char *scene_map, size_t *j)
{
	while (scene_map[*j] != ' ')
		(*j)++;
	while (scene_map[*j] == ' ')
		(*j)++;
}
