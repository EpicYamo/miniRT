/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle_intersections.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
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

static void	compute_cross_section(t_triangle_data *tri, t_vec3 right,
			t_vec3 tangent, t_vec3 *p0, t_vec3 *p1, t_vec3 *p2)
{
	double	s;
	double	r;
	double	third;

	s = tri->size;
	if (tri->kind == TRIANGLE_EQUILATERAL)
	{
		r = s / sqrt(3.0);
		*p0 = vec3_add(vec3_mul(right, r * cos(M_PI / 2.0)),
				vec3_mul(tangent, r * sin(M_PI / 2.0)));
		*p1 = vec3_add(vec3_mul(right,
					r * cos(M_PI / 2.0 + 2.0 * M_PI / 3.0)),
				vec3_mul(tangent, r * sin(M_PI / 2.0 + 2.0 * M_PI / 3.0)));
		*p2 = vec3_add(vec3_mul(right,
					r * cos(M_PI / 2.0 + 4.0 * M_PI / 3.0)),
				vec3_mul(tangent, r * sin(M_PI / 2.0 + 4.0 * M_PI / 3.0)));
	}
	else
	{
		third = s / 3.0;
		*p0 = vec3_add(vec3_mul(right, -third), vec3_mul(tangent, -third));
		*p1 = vec3_add(vec3_mul(right, 2.0 * third),
				vec3_mul(tangent, -third));
		*p2 = vec3_add(vec3_mul(right, -third),
				vec3_mul(tangent, 2.0 * third));
	}
}

static void	compute_prism_vertices(t_triangle_data *tri, t_vec3 *v)
{
	t_vec3	center;
	t_vec3	axis;
	t_vec3	right;
	t_vec3	tangent;
	t_vec3	p0;
	t_vec3	p1;
	t_vec3	p2;
	t_vec3	half_axis;

	center = vec3_create(tri->pos_x, tri->pos_y, tri->pos_z);
	axis = vec3_normalize(vec3_create(tri->vector_x, tri->vector_y,
				tri->vector_z));
	build_tangent_basis(axis, &right, &tangent);
	compute_cross_section(tri, right, tangent, &p0, &p1, &p2);
	half_axis = vec3_mul(axis, tri->depth / 2.0);
	v[0] = vec3_add(vec3_add(center, p0), half_axis);
	v[1] = vec3_add(vec3_add(center, p1), half_axis);
	v[2] = vec3_add(vec3_add(center, p2), half_axis);
	v[3] = vec3_sub(vec3_add(center, p0), half_axis);
	v[4] = vec3_sub(vec3_add(center, p1), half_axis);
	v[5] = vec3_sub(vec3_add(center, p2), half_axis);
}

static int	moller_trumbore(t_ray ray, t_vec3 v0, t_vec3 v1, t_vec3 v2,
			double *out_t, t_vec3 *out_normal)
{
	t_vec3	edge1;
	t_vec3	edge2;
	t_vec3	h;
	t_vec3	s;
	t_vec3	q;
	double	a;
	double	f;
	double	u;
	double	v;
	double	t;

	edge1 = vec3_sub(v1, v0);
	edge2 = vec3_sub(v2, v0);
	h = vec3_cross(ray.direction, edge2);
	a = vec3_dot(edge1, h);
	if (fabs(a) < EPSILON)
		return (0);
	f = 1.0 / a;
	s = vec3_sub(ray.origin, v0);
	u = f * vec3_dot(s, h);
	if (u < 0.0 || u > 1.0)
		return (0);
	q = vec3_cross(s, edge1);
	v = f * vec3_dot(ray.direction, q);
	if (v < 0.0 || u + v > 1.0)
		return (0);
	t = f * vec3_dot(edge2, q);
	if (t <= EPSILON)
		return (0);
	*out_t = t;
	*out_normal = vec3_normalize(vec3_cross(edge1, edge2));
	return (1);
}

static void	apply_triangle_checker(t_hit *hit, t_vec3 origin, t_vec3 normal)
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

static void	compute_triangle_uv(t_hit *hit, t_vec3 origin, t_vec3 normal,
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

static int	test_all_faces(t_ray ray, t_vec3 *v, double *best_t,
			t_vec3 *best_normal)
{
	int		found;
	double	t;
	t_vec3	n;
	int		i;
	int		idx[8][3];

	idx[0][0] = 0; idx[0][1] = 1; idx[0][2] = 2;
	idx[1][0] = 3; idx[1][1] = 4; idx[1][2] = 5;
	idx[2][0] = 0; idx[2][1] = 1; idx[2][2] = 3;
	idx[3][0] = 1; idx[3][1] = 4; idx[3][2] = 3;
	idx[4][0] = 1; idx[4][1] = 2; idx[4][2] = 4;
	idx[5][0] = 2; idx[5][1] = 5; idx[5][2] = 4;
	idx[6][0] = 2; idx[6][1] = 0; idx[6][2] = 5;
	idx[7][0] = 0; idx[7][1] = 3; idx[7][2] = 5;
	found = 0;
	*best_t = INFINITY;
	i = 0;
	while (i < 8)
	{
		if (moller_trumbore(ray, v[idx[i][0]], v[idx[i][1]], v[idx[i][2]],
				&t, &n) && t < *best_t)
		{
			*best_t = t;
			*best_normal = n;
			found = 1;
		}
		i++;
	}
	return (found);
}

int	intersect_triangle(t_ray ray, t_triangle_data *tri, t_hit *hit)
{
	t_vec3	v[6];
	double	best_t;
	t_vec3	best_normal;
	t_vec3	center;

	compute_prism_vertices(tri, v);
	if (!test_all_faces(ray, v, &best_t, &best_normal))
		return (0);
	hit->t = best_t;
	hit->point = vec3_add(ray.origin, vec3_mul(ray.direction, best_t));
	hit->normal = best_normal;
	if (vec3_dot(ray.direction, hit->normal) > 0.0)
		hit->normal = vec3_mul(hit->normal, -1.0);
	hit->red = tri->red;
	hit->green = tri->green;
	hit->blue = tri->blue;
	hit->shininess = tri->shininess;
	hit->specular_strength = tri->specular_strength;
	hit->texture_id = tri->texture_id;
	hit->bump_strength = tri->bump_strength;
	hit->checker = 0;
	center = vec3_create(tri->pos_x, tri->pos_y, tri->pos_z);
	if (tri->checker)
		apply_triangle_checker(hit, center, hit->normal);
	if (tri->texture_id >= 0)
		compute_triangle_uv(hit, center, hit->normal, tri->tex_repeat);
	return (1);
}
