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

static void	sample_pixel(t_scene *scene, int tex_id, double u, double v,
			unsigned int *r, unsigned int *g, unsigned int *b)
{
	t_texture		*tex;
	double			wu;
	double			wv;
	int				px;
	int				py;
	unsigned char	*pixel;

	tex = &scene->textures[tex_id];
	wu = u - floor(u);
	wv = v - floor(v);
	px = (int)(wu * tex->width);
	py = (int)(wv * tex->height);
	if (px < 0)
		px = 0;
	if (px >= tex->width)
		px = tex->width - 1;
	if (py < 0)
		py = 0;
	if (py >= tex->height)
		py = tex->height - 1;
	pixel = (unsigned char *)(tex->addr + py * tex->line
			+ px * (tex->bpp / 8));
	*b = pixel[0];
	*g = pixel[1];
	*r = pixel[2];
}

static double	sample_brightness(t_scene *scene, int tex_id, double u,
			double v)
{
	unsigned int	r;
	unsigned int	g;
	unsigned int	b;

	sample_pixel(scene, tex_id, u, v, &r, &g, &b);
	return ((r + g + b) / (3.0 * 255.0));
}

static void	build_normal_basis(t_vec3 n, t_vec3 *tangent, t_vec3 *bitangent)
{
	t_vec3	reference;

	reference = vec3_create(0.0, 1.0, 0.0);
	if (fabs(vec3_dot(n, reference)) > 0.99)
		reference = vec3_create(1.0, 0.0, 0.0);
	*tangent = vec3_normalize(vec3_cross(reference, n));
	*bitangent = vec3_cross(n, *tangent);
}

static void	apply_texture_and_bump(t_scene *scene, t_hit *hit)
{
	unsigned int	r;
	unsigned int	g;
	unsigned int	b;
	double			h_c;
	double			h_u;
	double			h_v;
	t_vec3			tangent;
	t_vec3			bitangent;
	t_vec3			perturb;

	sample_pixel(scene, hit->texture_id, hit->u, hit->v, &r, &g, &b);
	hit->red = r;
	hit->green = g;
	hit->blue = b;
	if (hit->bump_strength <= 0.0)
		return ;
	h_c = sample_brightness(scene, hit->texture_id, hit->u, hit->v);
	h_u = sample_brightness(scene, hit->texture_id, hit->u + 0.01, hit->v);
	h_v = sample_brightness(scene, hit->texture_id, hit->u, hit->v + 0.01);
	build_normal_basis(hit->normal, &tangent, &bitangent);
	perturb = vec3_add(vec3_mul(tangent, (h_u - h_c) * hit->bump_strength),
			vec3_mul(bitangent, (h_v - h_c) * hit->bump_strength));
	hit->normal = vec3_normalize(vec3_sub(hit->normal, perturb));
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

	if (hit->texture_id >= 0 && hit->texture_id < scene->texture_count)
		apply_texture_and_bump(scene, hit);
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
	i = 0;
	while (i < scene->element_counts.triangle_count)
	{
		if (intersect_triangle(shadow_ray, &scene->triangle_data[i], &hit))
			if (hit.t < light_dist)
				return (1);
		i++;
	}
	return (0);
}
