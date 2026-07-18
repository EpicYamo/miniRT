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

static void	build_tangent_basis(t_vec3 normal, t_vec3 *right, t_vec3 *tangent)
{
	t_vec3	reference;

	reference = vec3_create(0.0, 1.0, 0.0);
	if (fabs(vec3_dot(normal, reference)) > 0.99)
		reference = vec3_create(1.0, 0.0, 0.0);
	*right = vec3_normalize(vec3_cross(reference, normal));
	*tangent = vec3_cross(normal, *right);
}

static void	apply_plane_checker(t_hit *hit, t_vec3 origin, t_vec3 normal)
{
	t_vec3	right;
	t_vec3	tangent;
	t_vec3	rel;
	int		cell_u;
	int		cell_v;
	int		parity;

	build_tangent_basis(normal, &right, &tangent);
	rel = vec3_sub(hit->point, origin);
	cell_u = (int)floor(vec3_dot(rel, right) / CHECKER_CELL_SIZE);
	cell_v = (int)floor(vec3_dot(rel, tangent) / CHECKER_CELL_SIZE);
	parity = ((cell_u + cell_v) % 2 + 2) % 2;
	if (parity != 0)
	{
		hit->red = (unsigned int)(hit->red * 0.4);
		hit->green = (unsigned int)(hit->green * 0.4);
		hit->blue = (unsigned int)(hit->blue * 0.4);
	}
}

static void	compute_plane_uv(t_hit *hit, t_vec3 origin, t_vec3 normal,
			double repeat)
{
	t_vec3	right;
	t_vec3	tangent;
	t_vec3	rel;

	build_tangent_basis(normal, &right, &tangent);
	rel = vec3_sub(hit->point, origin);
	hit->u = (vec3_dot(rel, right) / TEXTURE_UNIT_SIZE) * repeat;
	hit->v = (vec3_dot(rel, tangent) / TEXTURE_UNIT_SIZE) * repeat;
}

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
	hit->shininess = plane->shininess;
	hit->specular_strength = plane->specular_strength;
	hit->texture_id = plane->texture_id;
	hit->bump_strength = plane->bump_strength;
	hit->checker = 0;
	if (plane->checker)
		apply_plane_checker(hit, point, normal);
	if (plane->texture_id >= 0)
		compute_plane_uv(hit, point, normal, plane->tex_repeat);
	return (1);
}
