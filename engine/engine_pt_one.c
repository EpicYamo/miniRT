/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_pt_one.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 16:42:20 by aaycan            #+#    #+#             */
/*   Updated: 2026/06/29 20:10:43 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include "../minilibx-linux/mlx.h"
#include <math.h>

static t_ray	generate_ray(t_scene *scene, int x, int y);

void	render_scene(t_rt *rt_this)
{
	int		x;
	int		y;
	t_ray	ray;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			ray = generate_ray(rt_this->old_data->scene, x, y);
			x++;
		}
		y++;
	}
}


static t_ray	generate_ray(t_scene *scene, int x, int y)
{
	t_ray	ray;
	t_vec3	cam_forward;
	t_vec3	world_up;
	t_vec3	cam_right;
	t_vec3	cam_up;
	double	scale;
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
	world_up = vec3_create(0.0, 1.0, 0.0);
	if (fabs(cam_forward.y) > 0.99)
		world_up = vec3_create(0.0, 0.0, 1.0);
	cam_right = vec3_normalize(vec3_cross(cam_forward, world_up));
	cam_up = vec3_cross(cam_right, cam_forward);
	scale = tan((scene->camera_data.fov * M_PI / 180.0) / 2.0);
	px = (2.0 * (x + 0.5) / WIDTH - 1.0) * scale;
	py = (1.0 - 2.0 * (y + 0.5) / HEIGHT) * scale;
	r_vector = vec3_mul(cam_right, px);
	u_vector = vec3_mul(cam_up, py);
	ray.direction = vec3_add(r_vector, u_vector);
	ray.direction = vec3_add(ray.direction, cam_forward);
	ray.direction = vec3_normalize(ray.direction);
	return (ray);
}
