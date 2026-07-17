/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ids.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 00:08:00 by aaycan            #+#    #+#             */
/*   Updated: 2026/07/18 00:08:13 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	assign_object_ids(t_rt *rt)
{
	t_scene	*scene;
	size_t	i;

	scene = rt->old_data->scene;
	i = 0;
	while (i < scene->element_counts.sphere_count)
	{
		scene->sphere_data[i].id = (int)i;
		i++;
	}
	i = 0;
	while (i < scene->element_counts.plane_count)
	{
		scene->plane_data[i].id = (int)i;
		i++;
	}
	i = 0;
	while (i < scene->element_counts.cylinder_count)
	{
		scene->cylinder_data[i].id = (int)i;
		i++;
	}
	rt->input.next_sphere_id = (int)scene->element_counts.sphere_count;
	rt->input.next_plane_id = (int)scene->element_counts.plane_count;
	rt->input.next_cylinder_id = (int)scene->element_counts.cylinder_count;
}

int	find_index_by_id(t_scene *scene, int type, int id)
{
	size_t	i;

	if (type == OBJ_SPHERE)
	{
		i = 0;
		while (i < scene->element_counts.sphere_count)
		{
			if (scene->sphere_data[i].id == id)
				return ((int)i);
			i++;
		}
	}
	else if (type == OBJ_PLANE)
	{
		i = 0;
		while (i < scene->element_counts.plane_count)
		{
			if (scene->plane_data[i].id == id)
				return ((int)i);
			i++;
		}
	}
	else if (type == OBJ_CYLINDER)
	{
		i = 0;
		while (i < scene->element_counts.cylinder_count)
		{
			if (scene->cylinder_data[i].id == id)
				return ((int)i);
			i++;
		}
	}
	return (-1);
}
