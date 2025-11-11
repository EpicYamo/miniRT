/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_pt_two.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 19:09:20 by aaycan            #+#    #+#             */
/*   Updated: 2025/11/11 23:34:25 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include "../minilibx-linux/mlx.h"
#include <stdlib.h>

int	exit_button(t_data *rt_data)
{
	mlx_destroy_window(rt_data->mlx_ptr, rt_data->mlx_window);
	mlx_destroy_display(rt_data->mlx_ptr);
	free(rt_data->mlx_ptr);
	free_scene();
	free(rt_data);
	exit(0);
}

int	exit_esc(int key, t_data *rt_data)
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
