/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane_intersections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 19:10:03 by aaycan            #+#    #+#             */
/*   Updated: 2026/07/07 19:13:03 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include <math.h>

int	intersect_plane(t_ray ray, t_plane_data *plane, t_hit *hit)
{
	t_vec3	point;
	t_vec3	normal;
	double	denom;
	double	t;

	point = vec3_create(plane->pos_x, plane->pos_y, plane->pos_z);
	normal = vec3_create(plane->vector_x, plane->vector_y, plane->vector_z);
	denom = vec3_dot(ray.direction, normal);
	if (fabs(denom) < EPSILON)
		return (0);
	t = vec3_dot(vec3_sub(point, ray.origin), normal) / denom;
	if (t <= EPSILON)
		return (0);
	if (denom > 0.0)
		normal = vec3_mul(normal, -1.0);
	hit->t = t;
	hit->point = vec3_add(ray.origin, vec3_mul(ray.direction, t));
	hit->normal = normal;
	hit->red = plane->red;
	hit->green = plane->green;
	hit->blue = plane->blue;
	return (1);
}
