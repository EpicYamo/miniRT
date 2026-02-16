/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_utils_pt_two.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 16:04:07 by aaycan            #+#    #+#             */
/*   Updated: 2026/02/15 16:33:55 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include <math.h>

double	vec3_dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

double	vec3_length(t_vec3 v)
{
	double	len;

	len = sqrt(vec3_dot(v, v));
	return (len);
}

t_vec3	vec3_normalize(t_vec3 v)
{
	double	len;

	len = vec3_length(v);
	if (len == 0.0)
		return (vec3_create(0, 0, 0));
	return (vec3_mul(v, 1 / len));
}
