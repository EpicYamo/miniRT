/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_pt_one.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 16:42:20 by aaycan            #+#    #+#             */
/*   Updated: 2026/07/17 17:55:49 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include "../minilibx-linux/mlx.h"
#include <math.h>

static t_ray	generate_ray(t_scene *scene, int x, int y);
static int		find_closest_hit(t_scene *scene, t_ray ray, t_hit *closest);
static int		color_to_int(unsigned int r, unsigned int g, unsigned int b);

void	render_scene(t_rt *rt_this, int step)
{
	int		x;
	int		y;
	int		bx;
	int		by;
	t_ray	ray;
	t_hit	closest;
	int		color;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			ray = generate_ray(rt_this->old_data->scene, x, y);
			if (find_closest_hit(rt_this->old_data->scene, ray, &closest))
			{
				if (closest.is_marker)
					color = color_to_int(255, 255, 255);
				else
					color = compute_color(rt_this->old_data->scene, &closest);
			}
			else
				color = color_to_int(0, 0, 0);
			by = 0;
			while (by < step && (y + by) < HEIGHT)
			{
				bx = 0;
				while (bx < step && (x + bx) < WIDTH)
				{
					my_mlx_pixel_put(&rt_this->img, x + bx, y + by, color);
					bx++;
				}
				by++;
			}
			x += step;
		}
		y += step;
	}
}

static t_ray	generate_ray(t_scene *scene, int x, int y)
{
	t_ray	ray;
	t_vec3	cam_forward;
	t_vec3	cam_right;
	t_vec3	cam_up;
	double	yaw;
	double	scale;
	double	aspect;
	double	px;
	double	py;
	t_vec3	r_vector;
	t_vec3	u_vector;

	ray.origin = vec3_create(scene->camera_data.pos_x,
								scene->camera_data.pos_y,
								scene->camera_data.pos_z);
	cam_forward = vec3_create(scene->camera_data.vector_x,
								scene->camera_data.vector_y,
								scene->camera_data.vector_z);
	cam_forward = vec3_normalize(cam_forward);
	yaw = atan2(cam_forward.x, cam_forward.z);
	cam_right = vec3_create(-cos(yaw), 0.0, sin(yaw));
	cam_up = vec3_cross(cam_right, cam_forward);
	scale = tan((scene->camera_data.fov * M_PI / 180.0) / 2.0);
	aspect = (double)WIDTH / (double)HEIGHT;
	px = (2.0 * (x + 0.5) / WIDTH - 1.0) * scale * aspect;
	py = (1.0 - 2.0 * (y + 0.5) / HEIGHT) * scale;
	r_vector = vec3_mul(cam_right, px);
	u_vector = vec3_mul(cam_up, py);
	ray.direction = vec3_add(r_vector, u_vector);
	ray.direction = vec3_add(ray.direction, cam_forward);
	ray.direction = vec3_normalize(ray.direction);
	return (ray);
}


static int	find_closest_hit(t_scene *scene, t_ray ray, t_hit *closest)
{
	size_t			i;
	t_hit			current;
	int				found;
	t_sphere_data	light_marker;

	found = 0;
	closest->t = INFINITY;
	closest->is_marker = 0;
	i = 0;
	while (i < scene->element_counts.sphere_count)
	{
		if (intersect_sphere(ray, &scene->sphere_data[i], &current)
			&& current.t < closest->t)
		{
			*closest = current;
			closest->is_marker = 0;
			found = 1;
		}
		i++;
	}
	i = 0;
	while (i < scene->element_counts.plane_count)
	{
		if (intersect_plane(ray, &scene->plane_data[i], &current)
			&& current.t < closest->t)
		{
			*closest = current;
			closest->is_marker = 0;
			found = 1;
		}
		i++;
	}
	i = 0;
	while (i < scene->element_counts.cylinder_count)
	{
		if (intersect_cylinder(ray, &scene->cylinder_data[i], &current)
			&& current.t < closest->t)
		{
			*closest = current;
			closest->is_marker = 0;
			found = 1;
		}
		i++;
	}
	light_marker.pos_x = scene->light_data.pos_x;
	light_marker.pos_y = scene->light_data.pos_y;
	light_marker.pos_z = scene->light_data.pos_z;
	light_marker.diameter = LIGHT_MARKER_DIAMETER;
	if (intersect_sphere(ray, &light_marker, &current)
		&& current.t < closest->t)
	{
		*closest = current;
		closest->is_marker = 1;
		found = 1;
	}
	return (found);
}

static int	color_to_int(unsigned int r, unsigned int g, unsigned int b)
{
	return ((r << 16) | (g << 8) | b);
}
