/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pt_four.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 14:03:46 by aaycan            #+#    #+#             */
/*   Updated: 2026/02/11 01:11:16 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include <stdlib.h>

static int		get_sign(const char **s);
static double	parse_fraction(const char **s);

void	increment_index_to_the_next_param(char *scene_map, size_t *j)
{
	while (scene_map[*j] != ' ')
		(*j)++;
	while (scene_map[*j] == ' ')
		(*j)++;
}

void	free_scene(void)
{
	t_scene	*scene;

	scene = parse_scene(NULL);
	free(scene->cylinder_data);
	free(scene->plane_data);
	free(scene->sphere_data);
	free(scene);
}

double	ft_atod(const char *s)
{
	double	res;
	int		sign;

	res = 0.0;
	sign = get_sign(&s);
	while ((*s) && (*s != '.') && (*s != ' ') && (*s != ','))
	{
		res = res * 10.0 + (*s - '0');
		s++;
	}
	res += parse_fraction(&s);
	return (res * sign);
}

static int	get_sign(const char **s)
{
	int	sign;

	sign = 1;
	if ((**s == '+') || (**s == '-'))
	{
		if (**s == '-')
			sign = -1;
		(*s)++;
	}
	return (sign);
}

static double	parse_fraction(const char **s)
{
	double	res;
	double	scale;

	res = 0.0;
	scale = 1.0;
	if (**s == '.')
	{
		(*s)++;
		while ((**s) && (**s != ' ') && (**s != ','))
		{
			scale *= 0.1;
			res += (**s - '0') * scale;
			(*s)++;
		}
	}
	return (res);
}
