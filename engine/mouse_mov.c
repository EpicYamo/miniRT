/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_mov.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 05:43:39 by aaycan            #+#    #+#             */
/*   Updated: 2026/07/17 17:23:44 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include "../minilibx-linux/mlx.h"
#include <math.h>

void	init_camera_state(t_rt *rt)
{
	t_vec3	forward;

	forward = vec3_normalize(vec3_create(
				rt->old_data->scene->camera_data.vector_x,
				rt->old_data->scene->camera_data.vector_y,
				rt->old_data->scene->camera_data.vector_z));
	rt->input.pitch = asin(forward.y);
	rt->input.yaw = atan2(forward.x, forward.z);
	rt->input.mode = MODE_RENDER;
	rt->input.render_step = 1;
	rt->input.right_mouse_held = 0;
	rt->input.last_time = get_time_seconds();
}

void	update_mouse_look(t_rt *rt)
{
	int		mouse_x;
	int		mouse_y;
	int		delta_x;
	int		delta_y;
	t_vec3	forward;

	if (!rt->input.right_mouse_held)
		return ;
	mlx_mouse_get_pos(rt->old_data->mlx_ptr, rt->old_data->mlx_window,
		&mouse_x, &mouse_y);
	delta_x = mouse_x - rt->input.last_mouse_x;
	delta_y = mouse_y - rt->input.last_mouse_y;
	rt->input.last_mouse_x = mouse_x;
	rt->input.last_mouse_y = mouse_y;
	if (delta_x == 0 && delta_y == 0)
		return ;
	rt->input.yaw -= delta_x * MOUSE_SENSITIVITY;
	rt->input.pitch -= delta_y * MOUSE_SENSITIVITY;
	if (rt->input.pitch > PITCH_LIMIT)
		rt->input.pitch = PITCH_LIMIT;
	if (rt->input.pitch < -PITCH_LIMIT)
		rt->input.pitch = -PITCH_LIMIT;
	forward.x = cos(rt->input.pitch) * sin(rt->input.yaw);
	forward.y = sin(rt->input.pitch);
	forward.z = cos(rt->input.pitch) * cos(rt->input.yaw);
	rt->old_data->scene->camera_data.vector_x = forward.x;
	rt->old_data->scene->camera_data.vector_y = forward.y;
	rt->old_data->scene->camera_data.vector_z = forward.z;
}
