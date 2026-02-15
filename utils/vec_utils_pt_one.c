/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_utils_pt_one.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 16:03:41 by aaycan            #+#    #+#             */
/*   Updated: 2026/02/15 16:13:54 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

t_vec3	vec3_create(double x, double y, double z)
{
	t_vec3	v;

	v.x = x;
	v.y = y;
	v.z = z;
	return (v);
}

t_vec3	vec3_add(t_vec3 a, t_vec3 b)
{
	t_vec3	v;

	v = vec3_create(a.x + b.x, a.y + b.y, a.z + b.z);
	return (v);
}

t_vec3	vec3_sub(t_vec3 a, t_vec3 b)
{
	t_vec3	v;

	v = vec3_create(a.x - b.x, a.y - b.y, a.z - b.z);
	return (v);
}

t_vec3	vec3_mul(t_vec3 v, double t)
{
	t_vec3	m;

	m = vec3_create(v.x * t, v.y * t, v.z * t);
	return (m);
}

t_vec3	vec3_normalize(t_vec3 v)
{
	double	len;

	len = vec3_length(v);
	if (len == 0.0)
		return (vec3_create(0, 0, 0));
	return (vec3_mul(v, 1 / len));
}
