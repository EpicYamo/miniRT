/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 11:40:19 by aaycan            #+#    #+#             */
/*   Updated: 2025/11/11 11:55:02 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include "../minilibx-linux/mlx.h"

void	run_engine(void)
{
	t_scene	*scene;

	scene = parse_scene(NULL);
	(void)scene;
}
