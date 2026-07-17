/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backup.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 23:04:50 by aaycan            #+#    #+#             */
/*   Updated: 2026/07/18 01:13:39 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include "../minilibx-linux/mlx.h"
#include <stdlib.h>
#include <string.h>

void	init_scene_backup(t_rt *rt)
{
	t_scene	*scene;
	size_t	sc;
	size_t	pc;
	size_t	cc;

	scene = rt->old_data->scene;
	sc = scene->element_counts.sphere_count;
	pc = scene->element_counts.plane_count;
	cc = scene->element_counts.cylinder_count;
	rt->backup.sphere_count = (int)sc;
	rt->backup.plane_count = (int)pc;
	rt->backup.cylinder_count = (int)cc;
	rt->backup.sphere_data = malloc(sizeof(t_sphere_data) * sc);
	if (!rt->backup.sphere_data)
	{
		mlx_destroy_window((*rt).old_data->mlx_ptr,
			(*rt).old_data->mlx_window);
		mlx_destroy_display((*rt).old_data->mlx_ptr);
		free((*rt).old_data->mlx_ptr);
		free_scene();
		free((*rt).old_data);
		error_message(1, "Malloc Error at init_scene_backup");
	}
	rt->backup.plane_data = malloc(sizeof(t_plane_data) * pc);
	if (!rt->backup.plane_data)
	{
		mlx_destroy_window((*rt).old_data->mlx_ptr,
			(*rt).old_data->mlx_window);
		mlx_destroy_display((*rt).old_data->mlx_ptr);
		free((*rt).old_data->mlx_ptr);
		free_scene();
		free(rt->backup.sphere_data);
		free((*rt).old_data);
		error_message(1, "Malloc Error at init_scene_backup");
	}
	rt->backup.cylinder_data = malloc(sizeof(t_cylinder_data) * cc);
	if (!rt->backup.cylinder_data)
	{
		mlx_destroy_window((*rt).old_data->mlx_ptr,
			(*rt).old_data->mlx_window);
		mlx_destroy_display((*rt).old_data->mlx_ptr);
		free((*rt).old_data->mlx_ptr);
		free_scene();
		free(rt->backup.sphere_data);
		free(rt->backup.plane_data);
		free((*rt).old_data);
		error_message(1, "Malloc Error at init_scene_backup");
	}
	if (sc > 0)
		memcpy(rt->backup.sphere_data, scene->sphere_data,
			sizeof(t_sphere_data) * sc);
	if (pc > 0)
		memcpy(rt->backup.plane_data, scene->plane_data,
			sizeof(t_plane_data) * pc);
	if (cc > 0)
		memcpy(rt->backup.cylinder_data, scene->cylinder_data,
			sizeof(t_cylinder_data) * cc);
}

void	free_scene_backup(t_rt *rt)
{
	free(rt->backup.sphere_data);
	free(rt->backup.plane_data);
	free(rt->backup.cylinder_data);
}

void	reset_position(t_rt *rt)
{
	int		type;
	int		id;
	int		index;
	t_vec3	orig;
	t_vec3	current;

	type = rt->input.selected_type;
	id = rt->input.selected_id;
	if (type == OBJ_NONE)
		return ;
	if ((type == OBJ_SPHERE && id >= rt->backup.sphere_count)
		|| (type == OBJ_PLANE && id >= rt->backup.plane_count)
		|| (type == OBJ_CYLINDER && id >= rt->backup.cylinder_count))
		return ;
	index = find_index_by_id(rt->old_data->scene, type, id);
	if (index == -1)
		return ;
	if (type == OBJ_SPHERE)
		orig = vec3_create(rt->backup.sphere_data[id].pos_x,
				rt->backup.sphere_data[id].pos_y,
				rt->backup.sphere_data[id].pos_z);
	else if (type == OBJ_PLANE)
		orig = vec3_create(rt->backup.plane_data[id].pos_x,
				rt->backup.plane_data[id].pos_y,
				rt->backup.plane_data[id].pos_z);
	else
		orig = vec3_create(rt->backup.cylinder_data[id].pos_x,
				rt->backup.cylinder_data[id].pos_y,
				rt->backup.cylinder_data[id].pos_z);
	current = get_object_center(rt->old_data->scene, type, index);
	push_undo(rt, type, id, UNDO_MOVE, current);
	set_object_center(rt->old_data->scene, type, index, orig);
}

void	reset_rotation(t_rt *rt)
{
	int		type;
	int		id;
	int		index;
	t_vec3	orig;
	t_vec3	current;

	type = rt->input.selected_type;
	id = rt->input.selected_id;
	if (type != OBJ_PLANE && type != OBJ_CYLINDER)
		return ;
	index = find_index_by_id(rt->old_data->scene, type, id);
	if (index == -1)
		return ;
	if (type == OBJ_PLANE)
		orig = vec3_create(rt->backup.plane_data[id].vector_x,
				rt->backup.plane_data[id].vector_y,
				rt->backup.plane_data[id].vector_z);
	else
		orig = vec3_create(rt->backup.cylinder_data[id].vector_x,
				rt->backup.cylinder_data[id].vector_y,
				rt->backup.cylinder_data[id].vector_z);
	current = get_object_direction(rt->old_data->scene, type, index);
	push_undo(rt, type, id, UNDO_ROTATE, current);
	set_object_direction(rt->old_data->scene, type, index, orig);
}
