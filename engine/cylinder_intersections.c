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

static void	build_tangent_basis(t_vec3 up, t_vec3 *right, t_vec3 *forward)
{
	t_vec3	reference;

	reference = vec3_create(0.0, 1.0, 0.0);
	if (fabs(vec3_dot(up, reference)) > 0.99)
		reference = vec3_create(1.0, 0.0, 0.0);
	*right = vec3_normalize(vec3_cross(reference, up));
	*forward = vec3_cross(up, *right);
}

static void	apply_flat_checker(t_hit *hit, t_vec3 origin, t_vec3 normal)
{
	t_vec3	right;
	t_vec3	forward;
	t_vec3	rel;
	int		cell_u;
	int		cell_v;
	int		parity;

	build_tangent_basis(normal, &right, &forward);
	rel = vec3_sub(hit->point, origin);
	cell_u = (int)floor(vec3_dot(rel, right) / CHECKER_CELL_SIZE);
	cell_v = (int)floor(vec3_dot(rel, forward) / CHECKER_CELL_SIZE);
	parity = ((cell_u + cell_v) % 2 + 2) % 2;
	if (parity != 0)
	{
		hit->red = (unsigned int)(hit->red * 0.4);
		hit->green = (unsigned int)(hit->green * 0.4);
		hit->blue = (unsigned int)(hit->blue * 0.4);
	}
}

static void	apply_side_checker(t_hit *hit, t_vec3 center, t_vec3 axis,
			double radius, double h)
{
	t_vec3	right;
	t_vec3	forward;
	t_vec3	rel;
	t_vec3	perp;
	double	theta;
	double	arc;
	int		cell_u;
	int		cell_v;
	int		parity;

	build_tangent_basis(axis, &right, &forward);
	rel = vec3_sub(hit->point, center);
	perp = vec3_sub(rel, vec3_mul(axis, h));
	theta = atan2(vec3_dot(perp, forward), vec3_dot(perp, right));
	arc = theta * radius;
	cell_u = (int)floor(arc / CHECKER_CELL_SIZE);
	cell_v = (int)floor(h / CHECKER_CELL_SIZE);
	parity = ((cell_u + cell_v) % 2 + 2) % 2;
	if (parity != 0)
	{
		hit->red = (unsigned int)(hit->red * 0.4);
		hit->green = (unsigned int)(hit->green * 0.4);
		hit->blue = (unsigned int)(hit->blue * 0.4);
	}
}

static void	compute_side_uv(t_hit *hit, t_vec3 center, t_vec3 axis,
			double radius, double h, double repeat)
{
	t_vec3	right;
	t_vec3	forward;
	t_vec3	rel;
	t_vec3	perp;
	double	theta;
	double	arc;

	build_tangent_basis(axis, &right, &forward);
	rel = vec3_sub(hit->point, center);
	perp = vec3_sub(rel, vec3_mul(axis, h));
	theta = atan2(vec3_dot(perp, forward), vec3_dot(perp, right));
	arc = theta * radius;
	hit->u = (arc / TEXTURE_UNIT_SIZE) * repeat;
	hit->v = (h / TEXTURE_UNIT_SIZE) * repeat;
}

static void	compute_flat_uv(t_hit *hit, t_vec3 origin, t_vec3 normal,
			double repeat)
{
	t_vec3	right;
	t_vec3	forward;
	t_vec3	rel;

	build_tangent_basis(normal, &right, &forward);
	rel = vec3_sub(hit->point, origin);
	hit->u = (vec3_dot(rel, right) / TEXTURE_UNIT_SIZE) * repeat;
	hit->v = (vec3_dot(rel, forward) / TEXTURE_UNIT_SIZE) * repeat;
}

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
		best.shininess = cy->shininess;
		best.specular_strength = cy->specular_strength;
		best.texture_id = cy->texture_id;
		best.bump_strength = cy->bump_strength;
		best.checker = 0;
		if (cy->checker)
		{
			if (best.side_hit)
				apply_side_checker(&best, center, axis,
					cy->diameter / 2.0, best.local_h);
			else
				apply_flat_checker(&best, best.point, best.normal);
		}
		if (cy->texture_id >= 0)
		{
			if (best.side_hit)
				compute_side_uv(&best, center, axis, cy->diameter / 2.0,
					best.local_h, cy->tex_repeat);
			else
				compute_flat_uv(&best, best.point, best.normal,
					cy->tex_repeat);
		}
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
	hit->side_hit = 1;
	hit->local_h = h;
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
	hit->side_hit = 0;
	return (1);
}
