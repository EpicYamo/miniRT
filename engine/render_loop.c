/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 05:27:14 by aaycan            #+#    #+#             */
/*   Updated: 2026/07/17 23:19:34 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include "../minilibx-linux/mlx.h"
#include <math.h>
#include <sys/time.h>

int	render_loop(t_rt *rt)
{
	if (rt->input.mode == MODE_NAVIGATE)
	{
		update_mouse_look(rt);
		update_movement(rt);
		update_drag(rt);
		render_scene(rt, rt->input.render_step);
		present_frame(rt);
	}
	return (0);
}

void	present_frame(t_rt *rt)
{
	mlx_put_image_to_window(rt->old_data->mlx_ptr,
		rt->old_data->mlx_window, rt->img.img_ptr, 0, 0);
	draw_text_input(rt);
}


double	get_time_seconds(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec + tv.tv_usec / 1000000.0);
}

void	update_movement(t_rt *rt)
{
	double	dt;
	double	now;
	t_vec3	forward;
	t_vec3	right;
	t_vec3	world_up;
	t_vec3	move;
	t_vec3	pos;
	double	speed;

	now = get_time_seconds();
	dt = now - rt->input.last_time;
	rt->input.last_time = now;
	forward = vec3_create(rt->old_data->scene->camera_data.vector_x,
			rt->old_data->scene->camera_data.vector_y,
			rt->old_data->scene->camera_data.vector_z);
	right = vec3_create(-cos(rt->input.yaw), 0.0, sin(rt->input.yaw));
	world_up = vec3_create(0.0, 1.0, 0.0);
	move = vec3_create(0.0, 0.0, 0.0);
	if (rt->input.key_w)
		move = vec3_add(move, forward);
	if (rt->input.key_s)
		move = vec3_sub(move, forward);
	if (rt->input.key_d)
		move = vec3_add(move, right);
	if (rt->input.key_a)
		move = vec3_sub(move, right);
	if (rt->input.key_e)
		move = vec3_add(move, world_up);
	if (rt->input.key_q)
		move = vec3_sub(move, world_up);
	if (vec3_length(move) > 1e-6)
		move = vec3_normalize(move);
	pos = vec3_create(rt->old_data->scene->camera_data.pos_x,
			rt->old_data->scene->camera_data.pos_y,
			rt->old_data->scene->camera_data.pos_z);
	speed = MOVE_SPEED;
	if (rt->input.key_shift)
		speed *= 2.0;
	pos = vec3_add(pos, vec3_mul(move, speed * dt));
	rt->old_data->scene->camera_data.pos_x = pos.x;
	rt->old_data->scene->camera_data.pos_y = pos.y;
	rt->old_data->scene->camera_data.pos_z = pos.z;
}
