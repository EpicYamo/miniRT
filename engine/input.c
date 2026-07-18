/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 05:16:05 by aaycan            #+#    #+#             */
/*   Updated: 2026/07/18 03:27:44 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include "../minilibx-linux/mlx.h"
#include <X11/X.h>
#include <X11/keysym.h>

int	key_press(int keycode, t_rt *rt)
{
	if (rt->input.text_input_mode)
	{
		handle_text_input_key(rt, keycode);
		if (rt->input.mode == MODE_RENDER)
		{
			render_scene(rt, 1);
			present_frame(rt);
		}
		return (0);
	}
	if (keycode == 'w')
		rt->input.key_w = 1;
	else if (keycode == 'a')
		rt->input.key_a = 1;
	else if (keycode == 's')
		rt->input.key_s = 1;
	else if (keycode == 'd')
		rt->input.key_d = 1;
	else if (keycode == 'q')
		rt->input.key_q = 1;
	else if (keycode == 'e')
		rt->input.key_e = 1;
	else if (keycode == 65307)
		handle_exit(rt);
	else if (keycode == 65505 || keycode == 65506)
		rt->input.key_shift = 1;
	else if (keycode == 65507 || keycode == 65508)
		rt->input.key_ctrl = 1;
	else if (keycode == 'z' && rt->input.key_ctrl)
	{
		undo_last(rt);
		if (rt->input.mode == MODE_RENDER)
		{
			render_scene(rt, 1);
			present_frame(rt);
		}
	}
	else if (keycode == 'j')
	{
		reset_position(rt);
		if (rt->input.mode == MODE_RENDER)
		{
			render_scene(rt, 1);
			present_frame(rt);
		}
	}
	else if (keycode == 'k')
	{
		reset_rotation(rt);
		if (rt->input.mode == MODE_RENDER)
		{
			render_scene(rt, 1);
			present_frame(rt);
		}
	}
	else if (keycode == 'n')
	{
		if (rt->input.dragging_axis != -1 && rt->input.edit_mode != EDIT_NONE)
		{
			rt->input.text_input_mode = 1;
			rt->input.text_target = TEXT_TARGET_AXIS;
			rt->input.text_len = 0;
			rt->input.text_buffer[0] = '\0';
		}
	}
	else if (keycode == 'p')
	{
		cycle_property_field(rt);
		if (rt->input.mode == MODE_RENDER)
		{
			render_scene(rt, 1);
			present_frame(rt);
		}
	}
	else if (keycode == 'b')
	{
		if (rt->input.selected_type != OBJ_NONE
			&& rt->input.active_property != PROP_NONE)
		{
			rt->input.text_input_mode = 1;
			rt->input.text_target = TEXT_TARGET_PROPERTY;
			rt->input.text_len = 0;
			rt->input.text_buffer[0] = '\0';
		}
	}
	else if (keycode == 65289)
	{
		if (rt->input.mode == MODE_RENDER)
		{
			rt->input.mode = MODE_NAVIGATE;
			rt->input.render_step = NAV_RENDER_STEP;
		}
		else
		{
			rt->input.mode = MODE_RENDER;
			rt->input.render_step = 1;
			render_scene(rt, 1);
			present_frame(rt);
		}
	}
	else if (keycode == '1' && rt->input.selected_type == OBJ_NONE)
		spawn_sphere(rt);
	else if (keycode == '2' && rt->input.selected_type == OBJ_NONE)
		spawn_plane(rt);
	else if (keycode == '3' && rt->input.selected_type == OBJ_NONE)
		spawn_cylinder(rt);
	else if (keycode == '4' && rt->input.selected_type == OBJ_NONE)
		spawn_light(rt);
	else if (keycode == '5' && rt->input.selected_type == OBJ_NONE)
		spawn_cube(rt);
	else if (keycode == 'y')
	{
		cycle_selected_light(rt);
		if (rt->input.mode == MODE_RENDER)
		{
			render_scene(rt, 1);
			present_frame(rt);
		}
	}
	else if (keycode == 65535)
		delete_selected(rt);
	else if (keycode == 'c')
	{
		rt->input.selected_type = OBJ_NONE;
		rt->input.selected_id = -1;
		rt->input.edit_mode = EDIT_NONE;
		rt->input.dragging_axis = -1;
		rt->input.active_property = PROP_NONE;
		if (rt->input.mode == MODE_RENDER)
		{
			render_scene(rt, 1);
			present_frame(rt);
		}
	}
	else if (keycode == 'm')
	{
		if (rt->input.selected_type != OBJ_NONE)
		{
			if (rt->input.edit_mode == EDIT_MOVE)
				rt->input.edit_mode = EDIT_NONE;
			else
				rt->input.edit_mode = EDIT_MOVE;
			if (rt->input.mode == MODE_RENDER)
			{
				render_scene(rt, 1);
				present_frame(rt);
			}
		}
	}
	else if (keycode == 'r')
	{
		if (rt->input.selected_type == OBJ_PLANE
			|| rt->input.selected_type == OBJ_CYLINDER
			|| rt->input.selected_type == OBJ_CUBE)
		{
			if (rt->input.edit_mode == EDIT_ROTATE)
				rt->input.edit_mode = EDIT_NONE;
			else
				rt->input.edit_mode = EDIT_ROTATE;
			if (rt->input.mode == MODE_RENDER)
			{
				render_scene(rt, 1);
				present_frame(rt);
			}
		}
	}
	return (0);
}

int	key_release(int keycode, t_rt *rt)
{
	if (keycode == 'w')
		rt->input.key_w = 0;
	else if (keycode == 'a')
		rt->input.key_a = 0;
	else if (keycode == 's')
		rt->input.key_s = 0;
	else if (keycode == 'd')
		rt->input.key_d = 0;
	else if (keycode == 'q')
		rt->input.key_q = 0;
	else if (keycode == 'e')
		rt->input.key_e = 0;
	else if (keycode == 65505 || keycode == 65506)
		rt->input.key_shift = 0;
	else if (keycode == 65507 || keycode == 65508)
		rt->input.key_ctrl = 0;
	return (0);
}

int	mouse_press(int button, int x, int y, t_rt *rt)
{
	int		obj_type;
	int		obj_id;
	int		mx;
	int		my;
	int		axis;
	int		sel_index;
	t_ray	ray;

	(void)x;
	(void)y;
	if (button == 3)
	{
		rt->input.right_mouse_held = 1;
		mlx_mouse_get_pos(rt->old_data->mlx_ptr, rt->old_data->mlx_window,
			&rt->input.last_mouse_x, &rt->input.last_mouse_y);
	}
	else if (button == 1)
	{
		mlx_mouse_get_pos(rt->old_data->mlx_ptr, rt->old_data->mlx_window,
			&mx, &my);
		axis = -1;
		if (rt->input.selected_type != OBJ_NONE)
		{
			if (rt->input.edit_mode == EDIT_MOVE)
				axis = pick_axis(rt, mx, my);
			else if (rt->input.edit_mode == EDIT_ROTATE)
				axis = pick_ring_axis(rt, mx, my);
		}
		sel_index = -1;
		if (axis != -1)
			sel_index = find_index_by_id(rt->old_data->scene,
					rt->input.selected_type, rt->input.selected_id);
		if (axis != -1 && sel_index != -1)
		{
			rt->input.left_mouse_held = 1;
			rt->input.dragging_axis = axis;
			if (rt->input.edit_mode == EDIT_MOVE)
			{
				rt->input.drag_origin = get_object_center(rt->old_data->scene,
						rt->input.selected_type, sel_index);
				if (axis == 0)
					rt->input.drag_axis_dir = vec3_create(1.0, 0.0, 0.0);
				else if (axis == 1)
					rt->input.drag_axis_dir = vec3_create(0.0, 1.0, 0.0);
				else
					rt->input.drag_axis_dir = vec3_create(0.0, 0.0, 1.0);
				ray = generate_ray(rt->old_data->scene, mx, my);
				rt->input.drag_start_t = compute_axis_t(ray,
						rt->input.drag_origin, rt->input.drag_axis_dir);
			}
			else
			{
				rt->input.drag_last_x = mx;
				rt->input.drag_last_y = my;
				rt->input.drag_start_dir = get_object_direction(
						rt->old_data->scene, rt->input.selected_type,
						sel_index);
			}
		}
		else
		{
			if (pick_object(rt->old_data->scene, mx, my,
					&obj_type, &obj_id))
			{
				rt->input.selected_type = obj_type;
				rt->input.selected_id = obj_id;
			}
			else
			{
				rt->input.selected_type = OBJ_NONE;
				rt->input.selected_id = -1;
			}
			rt->input.dragging_axis = -1;
			rt->input.edit_mode = EDIT_NONE;
			rt->input.active_property = PROP_NONE;
			render_scene(rt, rt->input.render_step);
			present_frame(rt);
		}
	}
	return (0);
}

int	mouse_release(int button, int x, int y, t_rt *rt)
{
	(void)x;
	(void)y;
	if (button == 3)
		rt->input.right_mouse_held = 0;
	else if (button == 1 && rt->input.left_mouse_held)
	{
		rt->input.left_mouse_held = 0;
		if (rt->input.dragging_axis != -1)
		{
			if (rt->input.edit_mode == EDIT_MOVE)
				push_undo(rt, rt->input.selected_type,
					rt->input.selected_id, 0, rt->input.drag_origin);
			else if (rt->input.edit_mode == EDIT_ROTATE)
				push_undo(rt, rt->input.selected_type,
					rt->input.selected_id, 1, rt->input.drag_start_dir);
		}
	}
	return (0);
}
