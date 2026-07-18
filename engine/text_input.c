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

static void	confirm_property_input(t_rt *rt)
{
	double	value;
	double	old;
	int		type;
	int		index;

	if (rt->input.text_len == 0)
	{
		rt->input.text_input_mode = 0;
		return ;
	}
	type = rt->input.selected_type;
	index = find_index_by_id(rt->old_data->scene, type,
			rt->input.selected_id);
	if (index == -1)
	{
		rt->input.text_input_mode = 0;
		return ;
	}
	value = ft_atod(rt->input.text_buffer);
	old = get_property_value(rt->old_data->scene, type, index,
			rt->input.active_property);
	push_property_undo(rt, type, rt->input.selected_id,
		rt->input.active_property, old);
	set_property_value(rt->old_data->scene, type, index,
		rt->input.active_property, value);
	rt->input.text_input_mode = 0;
}

static void	confirm_texture_input(t_rt *rt)
{
	int	type;
	int	index;
	int	old_id;
	int	new_id;

	type = rt->input.selected_type;
	index = find_index_by_id(rt->old_data->scene, type,
			rt->input.selected_id);
	if (index == -1)
	{
		rt->input.text_input_mode = 0;
		return ;
	}
	old_id = (int)get_property_value(rt->old_data->scene, type, index,
			PROP_TEXTURE);
	if (rt->input.text_len == 0)
		new_id = -1;
	else
		new_id = get_or_load_texture(rt, rt->input.text_buffer);
	push_property_undo(rt, type, rt->input.selected_id, PROP_TEXTURE,
		(double)old_id);
	set_property_value(rt->old_data->scene, type, index, PROP_TEXTURE,
		(double)new_id);
	rt->input.text_input_mode = 0;
}

void	confirm_text_input(t_rt *rt)
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

static int	is_filename_char(int c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9') || c == '.' || c == '_' || c == '-');
}

void	handle_text_input_key(t_rt *rt, int keycode)
{
	if (keycode == 65293)
	{
		if (rt->input.text_target == TEXT_TARGET_PROPERTY)
			confirm_property_input(rt);
		else if (rt->input.text_target == TEXT_TARGET_TEXTURE_NAME)
			confirm_texture_input(rt);
		else
			confirm_text_input(rt);
	}
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
	else if (rt->input.text_target == TEXT_TARGET_TEXTURE_NAME)
	{
		if (is_filename_char(keycode)
			&& rt->input.text_len < TEXT_BUFFER_SIZE - 1)
		{
			rt->input.text_buffer[rt->input.text_len] = (char)keycode;
			rt->input.text_len++;
			rt->input.text_buffer[rt->input.text_len] = '\0';
		}
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

static char	*property_field_label(int type, int field)
{
	if (field == PROP_COLOR_R)
		return ("Color R");
	if (field == PROP_COLOR_G)
		return ("Color G");
	if (field == PROP_COLOR_B)
		return ("Color B");
	if (field == PROP_SIZE1)
		return ("Diameter");
	if (field == PROP_SIZE2 && type == OBJ_TRIANGLE)
		return ("Depth");
	if (field == PROP_SIZE2)
		return ("Height");
	if (field == PROP_CHECKER)
		return ("Checker");
	if (field == PROP_SHININESS)
		return ("Shininess");
	if (field == PROP_SPEC_STRENGTH)
		return ("Specular");
	if (field == PROP_TEX_REPEAT)
		return ("Repeat");
	if (field == PROP_KIND)
		return ("Kind");
	if (field == PROP_BRIGHTNESS)
		return ("Brightness");
	return ("Bump");
}

void	draw_text_input(t_rt *rt)
{
	char	line[64];
	char	*mode_label;
	char	*axis_label;

	if (!rt->input.text_input_mode)
		return ;
	line[0] = '\0';
	if (rt->input.text_target == TEXT_TARGET_PROPERTY)
	{
		strcpy(line, property_field_label(rt->input.selected_type,
				rt->input.active_property));
	}
	else if (rt->input.text_target == TEXT_TARGET_TEXTURE_NAME)
	{
		strcpy(line, "Texture file");
	}
	else
	{
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
	}
	strcat(line, ": ");
	strcat(line, rt->input.text_buffer);
	mlx_string_put(rt->old_data->mlx_ptr, rt->old_data->mlx_window,
		20, 20, 0xFFFFFF, line);
}
