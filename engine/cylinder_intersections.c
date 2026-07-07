/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_intersections.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 19:25:54 by aaycan            #+#    #+#             */
/*   Updated: 2026/07/07 19:48:54 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include <math.h>

static int	check_side(t_ray ray, t_cylinder_data *cy, t_vec3 axis,
				t_vec3 center, t_hit *hit);
static int	check_cap(t_ray ray, t_vec3 cap_center, t_vec3 normal,
				double radius, t_hit *hit);

int	intersect_cylinder(t_ray ray, t_cylinder_data *cy, t_hit *hit)
{
	t_vec3	center;
	t_vec3	axis;
	t_vec3	top_center;
	t_vec3	bottom_center;
	t_hit	best;
	t_hit	current;
	int		found;

	center = vec3_create(cy->pos_x, cy->pos_y, cy->pos_z);
	axis = vec3_normalize(vec3_create(cy->vector_x, cy->vector_y, cy->vector_z));
	top_center = vec3_add(center, vec3_mul(axis, cy->height / 2.0));
	bottom_center = vec3_sub(center, vec3_mul(axis, cy->height / 2.0));
	found = 0;
	best.t = INFINITY;
	if (check_side(ray, cy, axis, center, &current) && current.t < best.t)
	{
		best = current;
		found = 1;
	}
	if (check_cap(ray, top_center, axis, cy->diameter / 2.0, &current)
		&& current.t < best.t)
	{
		best = current;
		found = 1;
	}
	if (check_cap(ray, bottom_center, vec3_mul(axis, -1.0),
			cy->diameter / 2.0, &current) && current.t < best.t)
	{
		best = current;
		found = 1;
	}
	if (found)
	{
		best.red = cy->red;
		best.green = cy->green;
		best.blue = cy->blue;
		*hit = best;
	}
	return (found);
}

static int	check_side(t_ray ray, t_cylinder_data *cy, t_vec3 axis,
				t_vec3 center, t_hit *hit)
{
	t_vec3	oc;
	t_vec3	oc_perp;
	t_vec3	d_perp;
	double	a;
	double	b;
	double	c;
	double	disc;
	double	t;
	double	h;

	oc = vec3_sub(ray.origin, center);
	oc_perp = vec3_sub(oc, vec3_mul(axis, vec3_dot(oc, axis)));
	d_perp = vec3_sub(ray.direction, vec3_mul(axis, vec3_dot(ray.direction, axis)));
	a = vec3_dot(d_perp, d_perp);
	if (a < EPSILON)
		return (0);
	b = 2.0 * vec3_dot(oc_perp, d_perp);
	c = vec3_dot(oc_perp, oc_perp) - (cy->diameter / 2.0) * (cy->diameter / 2.0);
	disc = b * b - 4.0 * a * c;
	if (disc < 0.0)
		return (0);
	t = (-b - sqrt(disc)) / (2.0 * a);
	h = vec3_dot(vec3_sub(vec3_add(ray.origin, vec3_mul(ray.direction, t)),
				center), axis);
	if (t <= EPSILON || fabs(h) > cy->height / 2.0)
	{
		t = (-b + sqrt(disc)) / (2.0 * a);
		h = vec3_dot(vec3_sub(vec3_add(ray.origin, vec3_mul(ray.direction, t)),
					center), axis);
		if (t <= EPSILON || fabs(h) > cy->height / 2.0)
			return (0);
	}
	hit->t = t;
	hit->point = vec3_add(ray.origin, vec3_mul(ray.direction, t));
	hit->normal = vec3_normalize(vec3_sub(vec3_sub(hit->point, center),
		vec3_mul(axis, h)));
	if (vec3_dot(ray.direction, hit->normal) > 0.0)
		hit->normal = vec3_mul(hit->normal, -1.0);
	return (1);
}

static int	check_cap(t_ray ray, t_vec3 cap_center, t_vec3 normal,
				double radius, t_hit *hit)
{
	double	denom;
	double	t;
	t_vec3	point;
	t_vec3	diff;

	denom = vec3_dot(ray.direction, normal);
	if (fabs(denom) < EPSILON)
		return (0);
	t = vec3_dot(vec3_sub(cap_center, ray.origin), normal) / denom;
	if (t <= EPSILON)
		return (0);
	point = vec3_add(ray.origin, vec3_mul(ray.direction, t));
	diff = vec3_sub(point, cap_center);
	if (vec3_dot(diff, diff) > radius * radius)
		return (0);
	hit->t = t;
	hit->point = point;
	if (denom > 0.0)
		hit->normal = vec3_mul(normal, -1.0);
	else
		hit->normal = normal;
	return (1);
}
