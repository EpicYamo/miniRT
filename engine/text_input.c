/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 23:16:29 by aaycan            #+#    #+#             */
/*   Updated: 2026/07/18 00:22:59 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include "../minilibx-linux/mlx.h"
#include <string.h>
#include <math.h>

t_vec3	get_axis_vector(int axis)
{
	if (axis == 0)
		return (vec3_create(1.0, 0.0, 0.0));
	if (axis == 1)
		return (vec3_create(0.0, 1.0, 0.0));
	return (vec3_create(0.0, 0.0, 1.0));
}

static void	confirm_text_input(t_rt *rt)
{
	double	value;
	t_vec3	axis_dir;
	t_vec3	current;
	int		type;
	int		index;

	if (rt->input.text_len == 0)
	{
		rt->input.text_input_mode = 0;
		return ;
	}
	value = ft_atod(rt->input.text_buffer);
	axis_dir = get_axis_vector(rt->input.dragging_axis);
	type = rt->input.selected_type;
	index = find_index_by_id(rt->old_data->scene, rt->input.selected_type,
			rt->input.selected_id);
	if (index == -1)
		return ;
	if (rt->input.edit_mode == EDIT_MOVE)
	{
		current = get_object_center(rt->old_data->scene, type, index);
		push_undo(rt, type, index, 0, current);
		current = vec3_add(current, vec3_mul(axis_dir, value));
		set_object_center(rt->old_data->scene, type, index, current);
	}
	else if (rt->input.edit_mode == EDIT_ROTATE)
	{
		current = get_object_direction(rt->old_data->scene, type, index);
		push_undo(rt, type, index, 1, current);
		current = vec3_normalize(rotate_vector(current, axis_dir,
					value * M_PI / 180.0));
		set_object_direction(rt->old_data->scene, type, index, current);
	}
	rt->input.text_input_mode = 0;
}

void	handle_text_input_key(t_rt *rt, int keycode)
{
	if (keycode == 65293)
		confirm_text_input(rt);
	else if (keycode == 65307)
	{
		rt->input.text_input_mode = 0;
		rt->input.text_len = 0;
	}
	else if (keycode == 65288)
	{
		if (rt->input.text_len > 0)
			rt->input.text_buffer[--rt->input.text_len] = '\0';
	}
	else if ((keycode >= '0' && keycode <= '9') || keycode == '-'
		|| keycode == '.')
	{
		if (rt->input.text_len < TEXT_BUFFER_SIZE - 1)
		{
			rt->input.text_buffer[rt->input.text_len] = (char)keycode;
			rt->input.text_len++;
			rt->input.text_buffer[rt->input.text_len] = '\0';
		}
	}
}

void	draw_text_input(t_rt *rt)
{
	char	line[64];
	char	*mode_label;
	char	*axis_label;

	if (!rt->input.text_input_mode)
		return ;
	if (rt->input.edit_mode == EDIT_MOVE)
		mode_label = "Move";
	else
		mode_label = "Rotate";
	if (rt->input.dragging_axis == 0)
		axis_label = "X";
	else if (rt->input.dragging_axis == 1)
		axis_label = "Y";
	else
		axis_label = "Z";
	strcpy(line, mode_label);
	strcat(line, " ");
	strcat(line, axis_label);
	strcat(line, ": ");
	strcat(line, rt->input.text_buffer);
	mlx_string_put(rt->old_data->mlx_ptr, rt->old_data->mlx_window,
		20, 20, 0xFFFFFF, line);
}
