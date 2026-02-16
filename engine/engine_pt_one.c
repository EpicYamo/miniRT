/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_pt_one.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 16:42:20 by aaycan            #+#    #+#             */
/*   Updated: 2026/02/16 21:15:31 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include "../minilibx-linux/mlx.h"
#include <math.h>

void	render_scene(t_data *data)
{
	int	x;
	int	y;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			render_pixel(data, x, y);
			x++;
		}
		y++;
	}
}

t_ray	generate_ray(t_scene *scene, int x, int y)
{
	t_ray	ray;
	double	px;
	double	py;
	double	fov_scale;
	double	aspect;

	aspect = (double)WIDTH / (double)HEIGHT;
	fov_scale = tan((scene->camera_data.fov * M_PI / 180.0) / 2.0);
	px = (2.0 * (x + 0.5) / WIDTH - 1.0) * aspect * fov_scale;
	py = (1.0 - 2.0 * (y + 0.5) / HEIGHT) * fov_scale;
	ray.origin = vec3_create(scene->camera_data.pos_x,
		scene->camera_data.pos_y,scene->camera_data.pos_z);
	ray.direction = vec3_normalize(vec3_create(px, py, 1.0));
	return (ray);
}

void	put_pixel(t_data *data, int x, int y, int color)
{
	mlx_pixel_put(data->mlx_ptr, data->mlx_window, x, y, color);
}

void	render_pixel(t_data *data, int x, int y)
{
	t_ray	ray;
	double	t;
	size_t	i;


	ray = generate_ray(data->scene, x, y);
	i = 0;
	while (i < data->scene->element_counts.sphere_count)
	{
		if (ray_sphere_intersect(ray, data->scene->sphere_data[i], &t))
			put_pixel(data, x, y, 0x00FF0000);
		i++;
	}
	
}

int	ray_sphere_intersect(t_ray ray, t_sphere_data sphere, double *t_hit)
{
	t_vec3	oc;
	t_vec3	sphere_v;
	double	a;
	double	b;
	double	c;
	double	discriminant;
	double	t;

	sphere_v = vec3_create(sphere.pos_x, sphere.pos_y, sphere.pos_z);
	oc = vec3_sub(ray.origin, sphere_v);
	a = vec3_dot(ray.direction, ray.direction);
	b = 2.0 * vec3_dot(oc, ray.direction);
	c = vec3_dot(oc, oc) - sphere.diameter * sphere.diameter;
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (0);
	t = (-b - sqrt(discriminant)) / (2.0 * a);
	if (t < 0)
		t = (-b + sqrt(discriminant)) / (2.0 * a);
	if (t < 0)
		return (0);
	*t_hit = t;
	return (1);
}
