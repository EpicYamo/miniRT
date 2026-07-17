/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_engine.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 11:40:19 by aaycan            #+#    #+#             */
/*   Updated: 2026/07/17 17:29:17 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include "../minilibx-linux/mlx.h"
#include <stdlib.h>

static void	init_engine_data(t_rt *rt_data);
static void	init_image_data(t_rt *rt_this);
static void	init_mlx_functions(t_rt *rt_this);

void	run_engine(void)
{
	t_rt	rt_this;

	init_engine_data(&rt_this);
	init_image_data(&rt_this);
	render_scene(&rt_this, 1);
	mlx_put_image_to_window(rt_this.old_data->mlx_ptr,
		rt_this.old_data->mlx_window, rt_this.img.img_ptr, 0, 0);
	init_camera_state(&rt_this);
	init_mlx_functions(&rt_this);
}

static void	init_engine_data(t_rt *rt_this)
{
	(*rt_this).old_data = malloc(sizeof(t_data));
	if (!(*rt_this).old_data)
	{
		free_scene();
		error_message(1, "Memory Allocation");
	}
	(*rt_this).old_data->scene = parse_scene(NULL);
	(*rt_this).old_data->mlx_ptr = mlx_init();
	if (!((*rt_this).old_data->mlx_ptr))
	{
		free_scene();
		free((*rt_this).old_data);
		error_message(1, "Memory Allocation");
	}
	(*rt_this).old_data->mlx_window
		= mlx_new_window((*rt_this).old_data->mlx_ptr, WIDTH, HEIGHT, "miniRT");
	if (!((*rt_this).old_data->mlx_window))
	{
		mlx_destroy_display((*rt_this).old_data->mlx_ptr);
		free((*rt_this).old_data->mlx_ptr);
		free_scene();
		free((*rt_this).old_data);
		error_message(1, "MLX Couldnt Create a New Window");
	}
}

static void	init_image_data(t_rt *rt_this)
{
	rt_this->img.img_ptr = mlx_new_image(rt_this->old_data->mlx_ptr, WIDTH, HEIGHT);
	if(!rt_this->img.img_ptr)
	{
		mlx_destroy_window((*rt_this).old_data->mlx_ptr,
			(*rt_this).old_data->mlx_window);
		mlx_destroy_display((*rt_this).old_data->mlx_ptr);
		free((*rt_this).old_data->mlx_ptr);
		free_scene();
		free((*rt_this).old_data);
		error_message(1, "MLX Couldnt Create a New Image");
	}
	rt_this->img.addr = mlx_get_data_addr(rt_this->img.img_ptr,
		&rt_this->img.bits_per_pixel, &rt_this->img.line_length, &rt_this->img.endian);
	if(!rt_this->img.addr)
	{
		mlx_destroy_image(rt_this->old_data->mlx_ptr, rt_this->img.img_ptr);
		mlx_destroy_window((*rt_this).old_data->mlx_ptr,
			(*rt_this).old_data->mlx_window);
		mlx_destroy_display((*rt_this).old_data->mlx_ptr);
		free((*rt_this).old_data->mlx_ptr);
		free_scene();
		free((*rt_this).old_data);
		error_message(1, "MLX Error Creating a Image Addr"); // maybe a better desc ?
	}
}

static void	init_mlx_functions(t_rt *rt_this)
{
	mlx_hook((*rt_this).old_data->mlx_window, 17, 0, handle_exit, rt_this);
	mlx_hook((*rt_this).old_data->mlx_window, 2, 1, key_press, rt_this);
	mlx_hook((*rt_this).old_data->mlx_window, 3, 1L << 1, key_release, rt_this);
	mlx_hook((*rt_this).old_data->mlx_window, 4, 1L << 2, mouse_press, rt_this);
	mlx_hook((*rt_this).old_data->mlx_window, 5, 1L << 3, mouse_release, rt_this);
	mlx_loop_hook((*rt_this).old_data->mlx_ptr, render_loop, rt_this);
	mlx_loop((*rt_this).old_data->mlx_ptr);
}


void	handle_exit(t_rt *rt_this)
{
	mlx_destroy_image(rt_this->old_data->mlx_ptr, rt_this->img.img_ptr);
	mlx_destroy_window(rt_this->old_data->mlx_ptr, rt_this->old_data->mlx_window);
	mlx_destroy_display(rt_this->old_data->mlx_ptr);
	free(rt_this->old_data->mlx_ptr);
	free_scene();
	free(rt_this->old_data);
	exit(0);
}
