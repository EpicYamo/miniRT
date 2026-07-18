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
#include <stdio.h>

static int		sel_mask_buffer[WIDTH * HEIGHT];
static int		find_closest_hit(t_scene *scene, t_ray ray, t_hit *closest);

static void	draw_loading_screen(t_rt *rt_this, int percent)
{
	char	line[32];

	mlx_clear_window(rt_this->old_data->mlx_ptr, rt_this->old_data->mlx_window);
	sprintf(line, "Rendering... %d%%", percent);
	mlx_string_put(rt_this->old_data->mlx_ptr, rt_this->old_data->mlx_window,
		WIDTH / 2 - 170, HEIGHT / 2, 0xFFFFFF, line);
	mlx_do_sync(rt_this->old_data->mlx_ptr);
}

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
	t_hit			hit;
	int				index;
	t_sphere_data	light_sphere;

	index = find_index_by_id(scene, type, id);
	if (index == -1)
		return (0);
	if (type == OBJ_SPHERE)
		return (intersect_sphere(ray, &scene->sphere_data[index], &hit));
	if (type == OBJ_CYLINDER)
		return (intersect_cylinder(ray, &scene->cylinder_data[index], &hit));
	if (type == OBJ_PLANE)
		return (test_plane_marker(ray, &scene->plane_data[index]));
	if (type == OBJ_LIGHT)
	{
		light_sphere.pos_x = scene->light_data[index].pos_x;
		light_sphere.pos_y = scene->light_data[index].pos_y;
		light_sphere.pos_z = scene->light_data[index].pos_z;
		light_sphere.diameter = scene->light_data[index].diameter;
		light_sphere.checker = 0;
		light_sphere.shininess = 0.0;
		light_sphere.specular_strength = 0.0;
		light_sphere.texture_id = -1;
		light_sphere.tex_repeat = 1.0;
		light_sphere.bump_strength = 0.0;
		return (intersect_sphere(ray, &light_sphere, &hit));
	}
	if (type == OBJ_CUBE)
		return (intersect_cube(ray, &scene->cube_data[index], &hit));
	if (type == OBJ_TRIANGLE)
		return (intersect_triangle(ray, &scene->triangle_data[index], &hit));
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

static int	shade_ray(t_scene *scene, t_ray ray, int high_quality)
{
	t_hit	closest;
	t_vec3	view_dir;

	if (find_closest_hit(scene, ray, &closest))
	{
		if (closest.obj_type == OBJ_LIGHT)
			return (color_to_int(closest.red, closest.green, closest.blue));
		view_dir = vec3_mul(ray.direction, -1.0);
		return (compute_color(scene, &closest, view_dir, high_quality));
	}
	return (color_to_int(0, 0, 0));
}

static int	supersample_pixel(t_scene *scene, int x, int y)
{
	int		sx;
	int		sy;
	int		color;
	double	fx;
	double	fy;
	double	acc[3];
	t_ray	ray;

	acc[0] = 0.0;
	acc[1] = 0.0;
	acc[2] = 0.0;
	sy = 0;
	while (sy < AA_SAMPLES)
	{
		sx = 0;
		while (sx < AA_SAMPLES)
		{
			fx = x + ((double)sx + 0.5) / AA_SAMPLES;
			fy = y + ((double)sy + 0.5) / AA_SAMPLES;
			ray = generate_ray_at(scene, fx, fy);
			color = shade_ray(scene, ray, 1);
			acc[0] += (color >> 16) & 0xFF;
			acc[1] += (color >> 8) & 0xFF;
			acc[2] += color & 0xFF;
			sx++;
		}
		sy++;
	}
	acc[0] /= (AA_SAMPLES * AA_SAMPLES);
	acc[1] /= (AA_SAMPLES * AA_SAMPLES);
	acc[2] /= (AA_SAMPLES * AA_SAMPLES);
	return (color_to_int((unsigned int)acc[0], (unsigned int)acc[1],
			(unsigned int)acc[2]));
}

void	render_scene(t_rt *rt_this, int step)
{
	int		x;
	int		y;
	int		bx;
	int		by;
	int		sel_hit;
	int		high_quality;
	int		last_percent;
	int		percent;
	t_ray	ray;
	t_scene	*scene;
	int		color;

	scene = rt_this->old_data->scene;
	high_quality = (step == 1);
	last_percent = -1;
	if (high_quality)
		draw_loading_screen(rt_this, 0);
	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			ray = generate_ray(scene, x, y);
			if (high_quality)
				color = supersample_pixel(scene, x, y);
			else
				color = shade_ray(scene, ray, high_quality);
			sel_hit = 0;
			if (rt_this->input.selected_type != OBJ_NONE)
				sel_hit = test_selected_hit(scene, ray,
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
		if (high_quality)
		{
			percent = (y * 100) / HEIGHT;
			if (percent != last_percent)
			{
				draw_loading_screen(rt_this, percent);
				last_percent = percent;
			}
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

t_ray	generate_ray_at(t_scene *scene, double fx, double fy)
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
	px = (2.0 * fx / WIDTH - 1.0) * scale * aspect;
	py = (1.0 - 2.0 * fy / HEIGHT) * scale;
	r_vector = vec3_mul(cam_right, px);
	u_vector = vec3_mul(cam_up, py);
	ray.direction = vec3_add(r_vector, u_vector);
	ray.direction = vec3_add(ray.direction, cam_forward);
	ray.direction = vec3_normalize(ray.direction);
	return (ray);
}

t_ray	generate_ray(t_scene *scene, int x, int y)
{
	return (generate_ray_at(scene, (double)x + 0.5, (double)y + 0.5));
}


static int	find_closest_hit(t_scene *scene, t_ray ray, t_hit *closest)
{
	size_t			i;
	t_hit			current;
	int				found;
	t_sphere_data	light_sphere;

	found = 0;
	closest->t = INFINITY;
	closest->obj_type = OBJ_NONE;
	i = 0;
	while (i < scene->element_counts.sphere_count)
	{
		if (intersect_sphere(ray, &scene->sphere_data[i], &current)
			&& current.t < closest->t)
		{
			*closest = current;
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
			closest->obj_type = OBJ_CYLINDER;
			closest->obj_index = (int)i;
			found = 1;
		}
		i++;
	}
	i = 0;
	while (i < scene->element_counts.cube_count)
	{
		if (intersect_cube(ray, &scene->cube_data[i], &current)
			&& current.t < closest->t)
		{
			*closest = current;
			closest->obj_type = OBJ_CUBE;
			closest->obj_index = (int)i;
			found = 1;
		}
		i++;
	}
	i = 0;
	while (i < scene->element_counts.triangle_count)
	{
		if (intersect_triangle(ray, &scene->triangle_data[i], &current)
			&& current.t < closest->t)
		{
			*closest = current;
			closest->obj_type = OBJ_TRIANGLE;
			closest->obj_index = (int)i;
			found = 1;
		}
		i++;
	}
	i = 0;
	while (i < scene->element_counts.light_count)
	{
		if (scene->light_data[i].diameter > 0.0)
		{
			light_sphere.pos_x = scene->light_data[i].pos_x;
			light_sphere.pos_y = scene->light_data[i].pos_y;
			light_sphere.pos_z = scene->light_data[i].pos_z;
			light_sphere.diameter = scene->light_data[i].diameter;
			light_sphere.red = scene->light_data[i].red;
			light_sphere.green = scene->light_data[i].green;
			light_sphere.blue = scene->light_data[i].blue;
			light_sphere.checker = 0;
			light_sphere.shininess = 0.0;
			light_sphere.specular_strength = 0.0;
			light_sphere.texture_id = -1;
			light_sphere.tex_repeat = 1.0;
			light_sphere.bump_strength = 0.0;
			if (intersect_sphere(ray, &light_sphere, &current)
				&& current.t < closest->t)
			{
				*closest = current;
				closest->obj_type = OBJ_LIGHT;
				closest->obj_index = (int)i;
				found = 1;
			}
		}
		i++;
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
		else if (hit.obj_type == OBJ_CYLINDER)
			*id = scene->cylinder_data[hit.obj_index].id;
		else if (hit.obj_type == OBJ_LIGHT)
			*id = scene->light_data[hit.obj_index].id;
		else if (hit.obj_type == OBJ_CUBE)
			*id = scene->cube_data[hit.obj_index].id;
		else
			*id = scene->triangle_data[hit.obj_index].id;
		return (1);
	}
	return (0);
}
