/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_intersections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 18:29:30 by aaycan            #+#    #+#             */
/*   Updated: 2026/07/07 19:48:22 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include <math.h>

int	intersect_sphere(t_ray ray, t_sphere_data *sphere, t_hit *hit)
{
	t_vec3	center;
	t_vec3	oc;
	double	radius;
	double	b_prime;
	double	c;
	double	discriminant;
	double	t;

	center = vec3_create(sphere->pos_x, sphere->pos_y, sphere->pos_z);
	radius = sphere->diameter / 2.0;
	oc = vec3_sub(ray.origin, center);
	b_prime = vec3_dot(oc, ray.direction);
	c = vec3_dot(oc, oc) - radius * radius;
	discriminant = b_prime * b_prime - c;
	if (discriminant < 0.0)
		return (0);
	t = -b_prime - sqrt(discriminant);
	if (t <= EPSILON)
		t = -b_prime + sqrt(discriminant);
	if (t <= EPSILON)
		return (0);
	hit->t = t;
	hit->point = vec3_add(ray.origin, vec3_mul(ray.direction, t));
	hit->normal = vec3_normalize(vec3_sub(hit->point, center));
	if (vec3_dot(ray.direction, hit->normal) > 0.0)
    	hit->normal = vec3_mul(hit->normal, -1.0);
	hit->red = sphere->red;
	hit->green = sphere->green;
	hit->blue = sphere->blue;
	hit->checker = sphere->checker;
	hit->shininess = sphere->shininess;
	hit->specular_strength = sphere->specular_strength;
	hit->has_texture = sphere->has_texture;
	return (1);
}
