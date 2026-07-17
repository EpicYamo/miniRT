/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gizmo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 19:17:01 by aaycan            #+#    #+#             */
/*   Updated: 2026/07/17 23:15:02 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include "../minilibx-linux/mlx.h"
#include <math.h>
#include <stdlib.h>

static void	get_axis_basis(int axis, t_vec3 *u, t_vec3 *v)
{
	if (axis == 0)
	{
		*u = vec3_create(0.0, 1.0, 0.0);
		*v = vec3_create(0.0, 0.0, 1.0);
	}
	else if (axis == 1)
	{
		*u = vec3_create(1.0, 0.0, 0.0);
		*v = vec3_create(0.0, 0.0, 1.0);
	}
	else
	{
		*u = vec3_create(1.0, 0.0, 0.0);
		*v = vec3_create(0.0, 1.0, 0.0);
	}
}

static void	draw_ring(t_rt *rt_this, t_vec3 center, int axis, int color)
{
	t_vec3	u;
	t_vec3	v;
	t_vec3	point;
	double	angle;
	int		i;
	int		px;
	int		py;
	int		prev_x;
	int		prev_y;
	int		has_prev;

	get_axis_basis(axis, &u, &v);
	has_prev = 0;
	prev_x = 0;
	prev_y = 0;
	i = 0;
	while (i <= RING_SEGMENTS)
	{
		angle = 2.0 * M_PI * i / RING_SEGMENTS;
		point = vec3_add(center, vec3_add(
					vec3_mul(u, cos(angle) * GIZMO_LENGTH),
					vec3_mul(v, sin(angle) * GIZMO_LENGTH)));
		if (project_point(rt_this->old_data->scene, point, &px, &py))
		{
			if (has_prev)
				draw_line_screen(rt_this, prev_x, prev_y, px, py, color);
			prev_x = px;
			prev_y = py;
			has_prev = 1;
		}
		else
			has_prev = 0;
		i++;
	}
}

t_vec3	get_object_center(t_scene *scene, int type, int index)
{
	if (type == OBJ_SPHERE)
		return (vec3_create(scene->sphere_data[index].pos_x,
				scene->sphere_data[index].pos_y,
				scene->sphere_data[index].pos_z));
	if (type == OBJ_CYLINDER)
		return (vec3_create(scene->cylinder_data[index].pos_x,
				scene->cylinder_data[index].pos_y,
				scene->cylinder_data[index].pos_z));
	if (type == OBJ_PLANE)
		return (vec3_create(scene->plane_data[index].pos_x,
				scene->plane_data[index].pos_y,
				scene->plane_data[index].pos_z));
	return (vec3_create(0.0, 0.0, 0.0));
}

static void	put_thick_pixel(t_rt *rt_this, int x, int y, int color)
{
	int	dx;
	int	dy;
	int	half;

	half = GIZMO_LINE_THICKNESS / 2;
	dy = -half;
	while (dy <= half)
	{
		dx = -half;
		while (dx <= half)
		{
			if (x + dx >= 0 && x + dx < WIDTH
				&& y + dy >= 0 && y + dy < HEIGHT)
				my_mlx_pixel_put(&rt_this->img, x + dx, y + dy, color);
			dx++;
		}
		dy++;
	}
}

void	draw_line_screen(t_rt *rt_this, int x0, int y0, int x1, int y1,
			int color)
{
	int	dx;
	int	dy;
	int	sx;
	int	sy;
	int	err;
	int	e2;

	dx = abs(x1 - x0);
	dy = -abs(y1 - y0);
	sx = -1 + 2 * (x0 < x1);
	sy = -1 + 2 * (y0 < y1);
	err = dx + dy;
	while (1)
	{
		put_thick_pixel(rt_this, x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break ;
		e2 = 2 * err;
		if (e2 >= dy)
		{
			err += dy;
			x0 += sx;
		}
		if (e2 <= dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

static void	draw_axis(t_rt *rt_this, t_vec3 center, t_vec3 dir, int color)
{
	t_vec3	tip;
	int		cx;
	int		cy;
	int		tx;
	int		ty;

	if (!project_point(rt_this->old_data->scene, center, &cx, &cy))
		return ;
	tip = vec3_add(center, vec3_mul(dir, GIZMO_LENGTH));
	if (project_point(rt_this->old_data->scene, tip, &tx, &ty))
		draw_line_screen(rt_this, cx, cy, tx, ty, color);
}

void	draw_gizmo(t_rt *rt_this)
{
	t_vec3	center;

	if (rt_this->input.selected_type == OBJ_NONE)
		return ;
	if (rt_this->input.edit_mode == EDIT_NONE)
		return ;
	center = get_object_center(rt_this->old_data->scene,
			rt_this->input.selected_type, rt_this->input.selected_index);
	if (rt_this->input.edit_mode == EDIT_MOVE)
	{
		draw_axis(rt_this, center, vec3_create(1.0, 0.0, 0.0),
			color_to_int(255, 60, 60));
		draw_axis(rt_this, center, vec3_create(0.0, 1.0, 0.0),
			color_to_int(60, 255, 60));
		draw_axis(rt_this, center, vec3_create(0.0, 0.0, 1.0),
			color_to_int(60, 60, 255));
	}
	else if (rt_this->input.edit_mode == EDIT_ROTATE)
	{
		draw_ring(rt_this, center, 0, color_to_int(255, 60, 60));
		draw_ring(rt_this, center, 1, color_to_int(60, 255, 60));
		draw_ring(rt_this, center, 2, color_to_int(60, 60, 255));
	}
}

int	project_point(t_scene *scene, t_vec3 point, int *out_x, int *out_y)
{
	t_vec3	cam_pos;
	t_vec3	cam_forward;
	t_vec3	cam_right;
	t_vec3	world_up;
	t_vec3	rel;
	double	cam_x;
	double	cam_y;
	double	cam_z;
	double	scale;
	double	aspect;

	cam_pos = vec3_create(scene->camera_data.pos_x,
			scene->camera_data.pos_y, scene->camera_data.pos_z);
	cam_forward = vec3_normalize(vec3_create(scene->camera_data.vector_x,
				scene->camera_data.vector_y, scene->camera_data.vector_z));
	world_up = vec3_create(0.0, 1.0, 0.0);
	cam_right = vec3_normalize(vec3_cross(cam_forward, world_up));
	rel = vec3_sub(point, cam_pos);
	cam_x = vec3_dot(rel, cam_right);
	cam_y = vec3_dot(rel, vec3_cross(cam_right, cam_forward));
	cam_z = vec3_dot(rel, cam_forward);
	if (cam_z <= 1e-4)
		return (0);
	scale = tan((scene->camera_data.fov * M_PI / 180.0) / 2.0);
	aspect = (double)WIDTH / (double)HEIGHT;
	*out_x = (int)(((cam_x / cam_z) / (scale * aspect) + 1.0) / 2.0 * WIDTH);
	*out_y = (int)((1.0 - (cam_y / cam_z) / scale) / 2.0 * HEIGHT);
	return (1);
}

void	set_object_center(t_scene *scene, int type, int index, t_vec3 pos)
{
	if (type == OBJ_SPHERE)
	{
		scene->sphere_data[index].pos_x = pos.x;
		scene->sphere_data[index].pos_y = pos.y;
		scene->sphere_data[index].pos_z = pos.z;
	}
	else if (type == OBJ_CYLINDER)
	{
		scene->cylinder_data[index].pos_x = pos.x;
		scene->cylinder_data[index].pos_y = pos.y;
		scene->cylinder_data[index].pos_z = pos.z;
	}
	else if (type == OBJ_PLANE)
	{
		scene->plane_data[index].pos_x = pos.x;
		scene->plane_data[index].pos_y = pos.y;
		scene->plane_data[index].pos_z = pos.z;
	}
}

static double	dist_point_segment(int px, int py, int x0, int y0,
				int x1, int y1)
{
	double	dx;
	double	dy;
	double	len2;
	double	t;
	double	projx;
	double	projy;

	dx = x1 - x0;
	dy = y1 - y0;
	len2 = dx * dx + dy * dy;
	if (len2 < 1e-6)
		return (sqrt((px - x0) * (px - x0) + (py - y0) * (py - y0)));
	t = ((px - x0) * dx + (py - y0) * dy) / len2;
	if (t < 0.0)
		t = 0.0;
	if (t > 1.0)
		t = 1.0;
	projx = x0 + t * dx;
	projy = y0 + t * dy;
	return (sqrt((px - projx) * (px - projx) + (py - projy) * (py - projy)));
}

int	pick_ring_axis(t_rt *rt, int mx, int my)
{
	t_vec3	center;
	t_vec3	u;
	t_vec3	v;
	t_vec3	point;
	double	angle;
	double	best_dist;
	int		best_axis;
	int		axis;
	int		i;
	int		px;
	int		py;
	int		prev_x;
	int		prev_y;
	int		has_prev;
	double	dist;

	center = get_object_center(rt->old_data->scene,
			rt->input.selected_type, rt->input.selected_index);
	best_dist = AXIS_PICK_RADIUS;
	best_axis = -1;
	axis = 0;
	while (axis < 3)
	{
		get_axis_basis(axis, &u, &v);
		has_prev = 0;
		prev_x = 0;
		prev_y = 0;
		i = 0;
		while (i <= RING_SEGMENTS)
		{
			angle = 2.0 * M_PI * i / RING_SEGMENTS;
			point = vec3_add(center, vec3_add(
						vec3_mul(u, cos(angle) * GIZMO_LENGTH),
						vec3_mul(v, sin(angle) * GIZMO_LENGTH)));
			if (project_point(rt->old_data->scene, point, &px, &py))
			{
				if (has_prev)
				{
					dist = dist_point_segment(mx, my, prev_x, prev_y, px, py);
					if (dist < best_dist)
					{
						best_dist = dist;
						best_axis = axis;
					}
				}
				prev_x = px;
				prev_y = py;
				has_prev = 1;
			}
			else
				has_prev = 0;
			i++;
		}
		axis++;
	}
	return (best_axis);
}

int	pick_axis(t_rt *rt, int mx, int my)
{
	t_vec3	center;
	t_vec3	dirs[3];
	t_vec3	tip;
	int		cx;
	int		cy;
	int		tx;
	int		ty;
	double	best_dist;
	int		best_axis;
	double	dist;
	int		i;

	center = get_object_center(rt->old_data->scene,
			rt->input.selected_type, rt->input.selected_index);
	dirs[0] = vec3_create(1.0, 0.0, 0.0);
	dirs[1] = vec3_create(0.0, 1.0, 0.0);
	dirs[2] = vec3_create(0.0, 0.0, 1.0);
	if (!project_point(rt->old_data->scene, center, &cx, &cy))
		return (-1);
	best_dist = AXIS_PICK_RADIUS;
	best_axis = -1;
	i = 0;
	while (i < 3)
	{
		tip = vec3_add(center, vec3_mul(dirs[i], GIZMO_LENGTH));
		if (project_point(rt->old_data->scene, tip, &tx, &ty))
		{
			dist = dist_point_segment(mx, my, cx, cy, tx, ty);
			if (dist < best_dist)
			{
				best_dist = dist;
				best_axis = i;
			}
		}
		i++;
	}
	return (best_axis);
}

double	compute_axis_t(t_ray ray, t_vec3 origin, t_vec3 axis)
{
	t_vec3	w0;
	double	b;
	double	d;
	double	e;
	double	denom;

	w0 = vec3_sub(origin, ray.origin);
	b = vec3_dot(axis, ray.direction);
	d = vec3_dot(axis, w0);
	e = vec3_dot(ray.direction, w0);
	denom = 1.0 - b * b;
	if (fabs(denom) < 1e-6)
		return (0.0);
	return ((b * e - d) / denom);
}

void	update_drag_move(t_rt *rt)
{
	int		mx;
	int		my;
	t_ray	ray;
	double	t;
	double	delta;
	t_vec3	new_point;

	mlx_mouse_get_pos(rt->old_data->mlx_ptr, rt->old_data->mlx_window,
		&mx, &my);
	ray = generate_ray(rt->old_data->scene, mx, my);
	t = compute_axis_t(ray, rt->input.drag_origin, rt->input.drag_axis_dir);
	delta = t - rt->input.drag_start_t;
	new_point = vec3_add(rt->input.drag_origin,
			vec3_mul(rt->input.drag_axis_dir, delta));
	set_object_center(rt->old_data->scene, rt->input.selected_type,
		rt->input.selected_index, new_point);
}

void	update_drag_rotate(t_rt *rt)
{
	int		mx;
	int		my;
	double	delta_x;
	double	angle;
	t_vec3	axis;
	t_vec3	dir;

	mlx_mouse_get_pos(rt->old_data->mlx_ptr, rt->old_data->mlx_window,
		&mx, &my);
	delta_x = mx - rt->input.drag_last_x;
	rt->input.drag_last_x = mx;
	rt->input.drag_last_y = my;
	if (fabs(delta_x) < 1e-9)
		return ;
	angle = delta_x * ROTATE_SENSITIVITY;
	if (rt->input.dragging_axis == 0)
		axis = vec3_create(1.0, 0.0, 0.0);
	else if (rt->input.dragging_axis == 1)
		axis = vec3_create(0.0, 1.0, 0.0);
	else
		axis = vec3_create(0.0, 0.0, 1.0);
	dir = get_object_direction(rt->old_data->scene, rt->input.selected_type,
			rt->input.selected_index);
	dir = vec3_normalize(rotate_vector(dir, axis, angle));
	set_object_direction(rt->old_data->scene, rt->input.selected_type,
		rt->input.selected_index, dir);
}

void	update_drag(t_rt *rt)
{
	if (!rt->input.left_mouse_held)
		return ;
	if (rt->input.dragging_axis == -1)
		return ;
	if (rt->input.edit_mode == EDIT_MOVE)
		update_drag_move(rt);
	else if (rt->input.edit_mode == EDIT_ROTATE)
		update_drag_rotate(rt);
}

t_vec3	rotate_vector(t_vec3 v, t_vec3 k, double angle)
{
	t_vec3	term1;
	t_vec3	term2;
	t_vec3	term3;

	term1 = vec3_mul(v, cos(angle));
	term2 = vec3_mul(vec3_cross(k, v), sin(angle));
	term3 = vec3_mul(k, vec3_dot(k, v) * (1.0 - cos(angle)));
	return (vec3_add(vec3_add(term1, term2), term3));
}

t_vec3	get_object_direction(t_scene *scene, int type, int index)
{
	if (type == OBJ_PLANE)
		return (vec3_create(scene->plane_data[index].vector_x,
				scene->plane_data[index].vector_y,
				scene->plane_data[index].vector_z));
	if (type == OBJ_CYLINDER)
		return (vec3_create(scene->cylinder_data[index].vector_x,
				scene->cylinder_data[index].vector_y,
				scene->cylinder_data[index].vector_z));
	return (vec3_create(0.0, 0.0, 1.0));
}

void	set_object_direction(t_scene *scene, int type, int index, t_vec3 dir)
{
	if (type == OBJ_PLANE)
	{
		scene->plane_data[index].vector_x = dir.x;
		scene->plane_data[index].vector_y = dir.y;
		scene->plane_data[index].vector_z = dir.z;
	}
	else if (type == OBJ_CYLINDER)
	{
		scene->cylinder_data[index].vector_x = dir.x;
		scene->cylinder_data[index].vector_y = dir.y;
		scene->cylinder_data[index].vector_z = dir.z;
	}
}
