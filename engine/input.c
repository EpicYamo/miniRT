/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 05:16:05 by aaycan            #+#    #+#             */
/*   Updated: 2026/07/17 18:00:34 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include "../minilibx-linux/mlx.h"
#include <X11/X.h>
#include <X11/keysym.h>


int	key_press(int keycode, t_rt *rt)
{
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
			mlx_put_image_to_window(rt->old_data->mlx_ptr,
				rt->old_data->mlx_window, rt->img.img_ptr, 0, 0);
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
	return (0);
}

int	mouse_press(int button, int x, int y, t_rt *rt)
{
	(void)x;
	(void)y;
	if (button == 3)
	{
		rt->input.right_mouse_held = 1;
		mlx_mouse_get_pos(rt->old_data->mlx_ptr, rt->old_data->mlx_window,
			&rt->input.last_mouse_x, &rt->input.last_mouse_y);
	}
	return (0);
}

int	mouse_release(int button, int x, int y, t_rt *rt)
{
	(void)x;
	(void)y;
	if (button == 3)
		rt->input.right_mouse_held = 0;
	return (0);
}
