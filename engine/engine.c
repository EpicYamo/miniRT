/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 16:42:20 by aaycan            #+#    #+#             */
/*   Updated: 2026/07/18 01:01:54 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include "../minilibx-linux/mlx.h"
#include <math.h>

static int		sel_mask_buffer[WIDTH * HEIGHT];
static int		find_closest_hit(t_scene *scene, t_ray ray, t_hit *closest);

static int	test_plane_marker(t_ray ray, t_plane_data *plane)
{
	t_vec3	point;
	t_vec3	normal;
	t_vec3	hit_point;
	t_vec3	diff;
	double	denom;
	double	t;

	point = vec3_create(plane->pos_x, plane->pos_y, plane->pos_z);
	normal = vec3_create(plane->vector_x, plane->vector_y, plane->vector_z);
	denom = vec3_dot(ray.direction, normal);
	if (fabs(denom) < 1e-4)
		return (0);
	t = vec3_dot(vec3_sub(point, ray.origin), normal) / denom;
	if (t <= 1e-4)
		return (0);
	hit_point = vec3_add(ray.origin, vec3_mul(ray.direction, t));
	diff = vec3_sub(hit_point, point);
	if (vec3_dot(diff, diff) > PLANE_MARKER_RADIUS * PLANE_MARKER_RADIUS)
		return (0);
	return (1);
}

static int	test_selected_hit(t_scene *scene, t_ray ray, int type, int id)
{
	t_hit	hit;
	int		index;

	index = find_index_by_id(scene, type, id);
	if (index == -1)
		return (0);
	if (type == OBJ_SPHERE)
		return (intersect_sphere(ray, &scene->sphere_data[index], &hit));
	if (type == OBJ_CYLINDER)
		return (intersect_cylinder(ray, &scene->cylinder_data[index], &hit));
	if (type == OBJ_PLANE)
		return (test_plane_marker(ray, &scene->plane_data[index]));
	return (0);
}

static int	is_edge_pixel_mask(int x, int y)
{
	int	dx;
	int	dy;
	int	nx;
	int	ny;

	dy = -OUTLINE_THICKNESS;
	while (dy <= OUTLINE_THICKNESS)
	{
		dx = -OUTLINE_THICKNESS;
		while (dx <= OUTLINE_THICKNESS)
		{
			nx = x + dx;
			ny = y + dy;
			if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT
				&& sel_mask_buffer[ny * WIDTH + nx] == 0)
				return (1);
			dx++;
		}
		dy++;
	}
	return (0);
}

static void	draw_selection_outline(t_rt *rt_this)
{
	int	x;
	int	y;

	if (rt_this->input.selected_type == OBJ_NONE)
		return ;
	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			if (sel_mask_buffer[y * WIDTH + x]
				&& is_edge_pixel_mask(x, y))
				my_mlx_pixel_put(&rt_this->img, x, y,
					color_to_int(255, 220, 0));
			x++;
		}
		y++;
	}
}

void	render_scene(t_rt *rt_this, int step)
{
	int		x;
	int		y;
	int		bx;
	int		by;
	int		sel_hit;
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
			sel_hit = 0;
			if (rt_this->input.selected_type != OBJ_NONE)
				sel_hit = test_selected_hit(rt_this->old_data->scene, ray,
						rt_this->input.selected_type,
						rt_this->input.selected_id);
			by = 0;
			while (by < step && (y + by) < HEIGHT)
			{
				bx = 0;
				while (bx < step && (x + bx) < WIDTH)
				{
					my_mlx_pixel_put(&rt_this->img, x + bx, y + by, color);
					sel_mask_buffer[(y + by) * WIDTH + (x + bx)] = sel_hit;
					bx++;
				}
				by++;
			}
			x += step;
		}
		y += step;
	}
	draw_selection_outline(rt_this);
	draw_gizmo(rt_this);
}

void	my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return ;
	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

t_ray	generate_ray(t_scene *scene, int x, int y)
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
	closest->obj_type = OBJ_NONE;
	i = 0;
	while (i < scene->element_counts.sphere_count)
	{
		if (intersect_sphere(ray, &scene->sphere_data[i], &current)
			&& current.t < closest->t)
		{
			*closest = current;
			closest->is_marker = 0;
			closest->obj_type = OBJ_SPHERE;
			closest->obj_index = (int)i;
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
			closest->obj_type = OBJ_PLANE;
			closest->obj_index = (int)i;
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
			closest->obj_type = OBJ_CYLINDER;
			closest->obj_index = (int)i;
			found = 1;
		}
		i++;
	}
	light_marker.pos_x = scene->light_data.pos_x;
	light_marker.pos_y = scene->light_data.pos_y;
	light_marker.pos_z = scene->light_data.pos_z;
	light_marker.diameter = LIGHT_MARKER_DIAMETER;
	light_marker.red = 0;
	light_marker.green = 0;
	light_marker.blue = 0;
	if (intersect_sphere(ray, &light_marker, &current) && current.t < closest->t)
	{
		*closest = current;
		closest->is_marker = 1;
		closest->obj_type = OBJ_NONE;
		found = 1;
	}
	return (found);
}

int	color_to_int(unsigned int r, unsigned int g, unsigned int b)
{
	return ((r << 16) | (g << 8) | b);
}


int	pick_object(t_scene *scene, int x, int y, int *type, int *id)
{
	t_ray	ray;
	t_hit	hit;

	ray = generate_ray(scene, x, y);
	if (find_closest_hit(scene, ray, &hit) && hit.obj_type != OBJ_NONE)
	{
		*type = hit.obj_type;
		if (hit.obj_type == OBJ_SPHERE)
			*id = scene->sphere_data[hit.obj_index].id;
		else if (hit.obj_type == OBJ_PLANE)
			*id = scene->plane_data[hit.obj_index].id;
		else
			*id = scene->cylinder_data[hit.obj_index].id;
		return (1);
	}
	return (0);
}
