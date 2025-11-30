/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_pt_one.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 11:40:19 by aaycan            #+#    #+#             */
/*   Updated: 2025/11/30 21:15:05 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include "../minilibx-linux/mlx.h"
#include <stdlib.h>
#include <stdio.h>

static void	init_engine_data(t_data **rt_data);
static void	init_mlx_functions(t_data *rt_data);
static int	exit_button(t_data *rt_data);
static int	exit_esc(int key, t_data *rt_data);

void	run_engine(void)
{
	t_data	*rt_data;

	init_engine_data(&rt_data);
	init_mlx_functions(rt_data);
	//ray_tracer(rt_data);
}

static void	init_engine_data(t_data **rt_data)
{
	*rt_data = malloc(sizeof(t_data));
	if (!(*rt_data))
	{
		free_scene();
		error_message(1, "Memory Allocation");
	}
	(*rt_data)->scene = parse_scene(NULL);
	(*rt_data)->mlx_ptr = mlx_init();
	if (!((*rt_data)->mlx_ptr))
	{
		free_scene();
		free((*rt_data));
		error_message(1, "Memory Allocation");
	}
	(*rt_data)->mlx_window
		= mlx_new_window((*rt_data)->mlx_ptr, 800, 600, "miniRT");
	if (!((*rt_data)->mlx_window))
	{
		mlx_destroy_display((*rt_data)->mlx_ptr);
		free((*rt_data)->mlx_ptr);
		free_scene();
		free((*rt_data));
		error_message(1, "MLX Couldnt Create a New Window");
	}
}

static void	init_mlx_functions(t_data *rt_data)
{
	mlx_hook(rt_data->mlx_window, 17, 0, exit_button, rt_data);
	mlx_hook(rt_data->mlx_window, 2, 1, exit_esc, rt_data);
	mlx_loop(rt_data->mlx_ptr);
}

static int	exit_button(t_data *rt_data)
{
	mlx_destroy_window(rt_data->mlx_ptr, rt_data->mlx_window);
	mlx_destroy_display(rt_data->mlx_ptr);
	free(rt_data->mlx_ptr);
	free_scene();
	free(rt_data);
	exit(0);
}

static int	exit_esc(int key, t_data *rt_data)
{
	if (key == 65307)
	{
		mlx_destroy_window(rt_data->mlx_ptr, rt_data->mlx_window);
		mlx_destroy_display(rt_data->mlx_ptr);
		free(rt_data->mlx_ptr);
		free_scene();
		free(rt_data);
		exit(0);
	}
	else
		return (0);
}
