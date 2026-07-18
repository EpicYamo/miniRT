/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lightning.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
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

static void	apply_checker(t_hit *hit)
{
	int	cx;
	int	cy;
	int	cz;
	int	parity;

	if (!hit->checker)
		return ;
	cx = (int)floor(hit->point.x / CHECKER_CELL_SIZE);
	cy = (int)floor(hit->point.y / CHECKER_CELL_SIZE);
	cz = (int)floor(hit->point.z / CHECKER_CELL_SIZE);
	parity = ((cx + cy + cz) % 2 + 2) % 2;
	if (parity != 0)
	{
		hit->red = (unsigned int)(hit->red * 0.4);
		hit->green = (unsigned int)(hit->green * 0.4);
		hit->blue = (unsigned int)(hit->blue * 0.4);
	}
}

static void	sample_texture(t_scene *scene, t_hit *hit)
{
	double			u;
	double			v;
	double			ny;
	int				px;
	int				py;
	unsigned char	*pixel;

	if (!hit->has_texture || !scene->tex_loaded)
		return ;
	u = (atan2(hit->normal.z, hit->normal.x) + M_PI) / (2.0 * M_PI);
	ny = hit->normal.y;
	if (ny < -1.0)
		ny = -1.0;
	if (ny > 1.0)
		ny = 1.0;
	v = acos(ny) / M_PI;
	px = (int)(u * scene->tex_width);
	py = (int)(v * scene->tex_height);
	if (px < 0)
		px = 0;
	if (px >= scene->tex_width)
		px = scene->tex_width - 1;
	if (py < 0)
		py = 0;
	if (py >= scene->tex_height)
		py = scene->tex_height - 1;
	pixel = (unsigned char *)(scene->tex_addr + py * scene->tex_line
			+ px * (scene->tex_bpp / 8));
	hit->blue = pixel[0];
	hit->green = pixel[1];
	hit->red = pixel[2];
}

static void	accumulate_light(t_scene *scene, t_hit *hit, t_light_data *light,
			t_vec3 view_dir, int high_quality, double *acc)
{
	t_vec3	light_pos;
	t_vec3	light_dir;
	t_vec3	reflect_dir;
	double	diffuse;
	double	spec;

	light_pos = vec3_create(light->pos_x, light->pos_y, light->pos_z);
	if (is_in_shadow(scene, hit->point, hit->normal, light_pos))
		return ;
	light_dir = vec3_normalize(vec3_sub(light_pos, hit->point));
	diffuse = vec3_dot(hit->normal, light_dir);
	if (diffuse < 0.0)
		diffuse = 0.0;
	diffuse *= light->brightness;
	acc[0] += diffuse * (light->red / 255.0);
	acc[1] += diffuse * (light->green / 255.0);
	acc[2] += diffuse * (light->blue / 255.0);
	if (!high_quality)
		return ;
	reflect_dir = vec3_sub(vec3_mul(hit->normal,
				2.0 * vec3_dot(hit->normal, light_dir)), light_dir);
	spec = vec3_dot(vec3_normalize(reflect_dir), view_dir);
	if (spec < 0.0)
		spec = 0.0;
	spec = pow(spec, hit->shininess) * hit->specular_strength
		* light->brightness;
	acc[0] += spec * (light->red / 255.0);
	acc[1] += spec * (light->green / 255.0);
	acc[2] += spec * (light->blue / 255.0);
}

int	compute_color(t_scene *scene, t_hit *hit, t_vec3 view_dir,
			int high_quality)
{
	double	acc[3];
	double	amb_r;
	double	amb_g;
	double	amb_b;
	double	final_r;
	double	final_g;
	double	final_b;
	size_t	light_limit;
	size_t	i;

	if (hit->has_texture)
		sample_texture(scene, hit);
	else
		apply_checker(hit);
	acc[0] = 0.0;
	acc[1] = 0.0;
	acc[2] = 0.0;
	light_limit = scene->element_counts.light_count;
	if (!high_quality && light_limit > NAV_LIGHT_LIMIT)
		light_limit = NAV_LIGHT_LIMIT;
	i = 0;
	while (i < light_limit)
	{
		accumulate_light(scene, hit, &scene->light_data[i], view_dir,
			high_quality, acc);
		i++;
	}
	amb_r = scene->ambient_data.ratio * scene->ambient_data.red / 255.0;
	amb_g = scene->ambient_data.ratio * scene->ambient_data.green / 255.0;
	amb_b = scene->ambient_data.ratio * scene->ambient_data.blue / 255.0;
	final_r = clamp01((hit->red / 255.0) * (amb_r + acc[0])) * 255.0;
	final_g = clamp01((hit->green / 255.0) * (amb_g + acc[1])) * 255.0;
	final_b = clamp01((hit->blue / 255.0) * (amb_b + acc[2])) * 255.0;
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
	i = 0;
	while (i < scene->element_counts.cube_count)
	{
		if (intersect_cube(shadow_ray, &scene->cube_data[i], &hit))
			if (hit.t < light_dist)
				return (1);
		i++;
	}
	return (0);
}
