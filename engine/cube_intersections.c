/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube_intersections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include <math.h>

static void	build_basis(t_vec3 up, t_vec3 *right, t_vec3 *forward)
{
	t_vec3	reference;

	reference = vec3_create(0.0, 1.0, 0.0);
	if (fabs(vec3_dot(up, reference)) > 0.99)
		reference = vec3_create(1.0, 0.0, 0.0);
	*right = vec3_normalize(vec3_cross(reference, up));
	*forward = vec3_cross(up, *right);
}

static int	slab_axis(double lo, double ld, double half, double *out)
{
	double	inv_d;
	double	tmp;

	if (fabs(ld) < 1e-9)
	{
		out[0] = -INFINITY;
		out[1] = INFINITY;
		out[2] = 0.0;
		out[3] = 0.0;
		if (lo < -half || lo > half)
			return (0);
		return (1);
	}
	inv_d = 1.0 / ld;
	out[0] = (-half - lo) * inv_d;
	out[1] = (half - lo) * inv_d;
	out[2] = -1.0;
	out[3] = 1.0;
	if (inv_d < 0.0)
	{
		tmp = out[0];
		out[0] = out[1];
		out[1] = tmp;
		tmp = out[2];
		out[2] = out[3];
		out[3] = tmp;
	}
	return (1);
}

static int	slab_test(double *lo, double *ld, double half, double *res)
{
	double	out[4];
	int		i;

	res[0] = -INFINITY;
	res[1] = INFINITY;
	i = 0;
	while (i < 3)
	{
		if (!slab_axis(lo[i], ld[i], half, out))
			return (0);
		if (out[0] > res[0])
		{
			res[0] = out[0];
			res[2] = (double)i;
			res[4] = out[2];
		}
		if (out[1] < res[1])
		{
			res[1] = out[1];
			res[3] = (double)i;
			res[5] = out[3];
		}
		if (res[1] < res[0])
			return (0);
		i++;
	}
	return (1);
}

int	intersect_cube(t_ray ray, t_cube_data *cube, t_hit *hit)
{
	t_vec3	center;
	t_vec3	up;
	t_vec3	right;
	t_vec3	forward;
	t_vec3	basis[3];
	t_vec3	oc;
	double	lo[3];
	double	ld[3];
	double	res[6];
	double	t;
	double	sign;
	int		axis;

	center = vec3_create(cube->pos_x, cube->pos_y, cube->pos_z);
	up = vec3_normalize(vec3_create(cube->vector_x, cube->vector_y,
				cube->vector_z));
	build_basis(up, &right, &forward);
	basis[0] = right;
	basis[1] = up;
	basis[2] = forward;
	oc = vec3_sub(ray.origin, center);
	lo[0] = vec3_dot(oc, right);
	lo[1] = vec3_dot(oc, up);
	lo[2] = vec3_dot(oc, forward);
	ld[0] = vec3_dot(ray.direction, right);
	ld[1] = vec3_dot(ray.direction, up);
	ld[2] = vec3_dot(ray.direction, forward);
	if (!slab_test(lo, ld, cube->size / 2.0, res))
		return (0);
	if (res[0] > EPSILON)
	{
		t = res[0];
		axis = (int)res[2];
		sign = res[4];
	}
	else if (res[1] > EPSILON)
	{
		t = res[1];
		axis = (int)res[3];
		sign = res[5];
	}
	else
		return (0);
	hit->t = t;
	hit->point = vec3_add(ray.origin, vec3_mul(ray.direction, t));
	hit->normal = vec3_normalize(vec3_mul(basis[axis], sign));
	if (vec3_dot(ray.direction, hit->normal) > 0.0)
		hit->normal = vec3_mul(hit->normal, -1.0);
	hit->red = cube->red;
	hit->green = cube->green;
	hit->blue = cube->blue;
	return (1);
}
