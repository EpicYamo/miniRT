/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lightning.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 18:53:13 by aaycan            #+#    #+#             */
/*   Updated: 2026/07/07 19:32:16 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include <math.h>

#define SHADOW_BIAS 1e-4

static double	clamp01(double v)
{
	if (v < 0.0)
		return (0.0);
	if (v > 1.0)
		return (1.0);
	return (v);
}

int	compute_color(t_scene *scene, t_hit *hit)
{
	t_vec3	light_pos;
	t_vec3	light_dir;
	double	diffuse;
	double	amb_r;
	double	amb_g;
	double	amb_b;
	double	final_r;
	double	final_g;
	double	final_b;

	light_pos = vec3_create(scene->light_data.pos_x,
			scene->light_data.pos_y, scene->light_data.pos_z);
	diffuse = 0.0;
	if (!is_in_shadow(scene, hit->point, hit->normal, light_pos))
	{
		light_dir = vec3_normalize(vec3_sub(light_pos, hit->point));
		diffuse = vec3_dot(hit->normal, light_dir);
		if (diffuse < 0.0)
			diffuse = 0.0;
		diffuse *= scene->light_data.brightness;
	}
	amb_r = scene->ambient_data.ratio * scene->ambient_data.red / 255.0;
	amb_g = scene->ambient_data.ratio * scene->ambient_data.green / 255.0;
	amb_b = scene->ambient_data.ratio * scene->ambient_data.blue / 255.0;
	final_r = clamp01((hit->red / 255.0) * (amb_r + diffuse)) * 255.0;
	final_g = clamp01((hit->green / 255.0) * (amb_g + diffuse)) * 255.0;
	final_b = clamp01((hit->blue / 255.0) * (amb_b + diffuse)) * 255.0;
	return (((unsigned int)final_r << 16)
		| ((unsigned int)final_g << 8) | (unsigned int)final_b);
}

int	is_in_shadow(t_scene *scene, t_vec3 point, t_vec3 normal, t_vec3 light_pos)
{
	t_ray	shadow_ray;
	t_hit	hit;
	double	light_dist;
	size_t	i;

	shadow_ray.origin = vec3_add(point, vec3_mul(normal, SHADOW_BIAS));
	shadow_ray.direction = vec3_normalize(vec3_sub(light_pos, point));
	light_dist = vec3_length(vec3_sub(light_pos, point));
	i = 0;
	while (i < scene->element_counts.sphere_count)
	{
		if (intersect_sphere(shadow_ray, &scene->sphere_data[i], &hit))
			if (hit.t < light_dist)
				return (1);
		i++;
	}
	i = 0;
	while (i < scene->element_counts.plane_count)
	{
		if (intersect_plane(shadow_ray, &scene->plane_data[i], &hit))
			if (hit.t < light_dist)
				return (1);
		i++;
	}
	i = 0;
	while (i < scene->element_counts.cylinder_count)
	{
		if (intersect_cylinder(shadow_ray, &scene->cylinder_data[i], &hit))
			if (hit.t < light_dist)
				return (1);
		i++;
	}
	return (0);
}
