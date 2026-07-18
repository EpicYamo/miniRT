/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   undo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 23:12:06 by aaycan            #+#    #+#             */
/*   Updated: 2026/07/18 03:27:37 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include <stdlib.h>

void	push_undo(t_rt *rt, int type, int id, int is_rotation,
			t_vec3 old_value)
{
	if (rt->undo_count == UNDO_CAPACITY)
		shift_undo_stack(rt);
	rt->undo_stack[rt->undo_count].action = is_rotation;
	rt->undo_stack[rt->undo_count].obj_type = type;
	rt->undo_stack[rt->undo_count].obj_id = id;
	rt->undo_stack[rt->undo_count].old_value = old_value;
	rt->undo_count++;
}

void	push_property_undo(t_rt *rt, int type, int id, int field,
			double old_scalar)
{
	if (rt->undo_count == UNDO_CAPACITY)
		shift_undo_stack(rt);
	rt->undo_stack[rt->undo_count].action = UNDO_PROPERTY;
	rt->undo_stack[rt->undo_count].obj_type = type;
	rt->undo_stack[rt->undo_count].obj_id = id;
	rt->undo_stack[rt->undo_count].field = field;
	rt->undo_stack[rt->undo_count].old_scalar = old_scalar;
	rt->undo_count++;
}

void	undo_last(t_rt *rt)
{
	t_undo_entry	*e;
	t_scene			*scene;
	int				index;

	if (rt->undo_count == 0)
		return ;
	rt->undo_count--;
	e = &rt->undo_stack[rt->undo_count];
	scene = rt->old_data->scene;
	if (e->action == UNDO_SPAWN)
	{
		index = find_index_by_id(scene, e->obj_type, e->obj_id);
		if (index != -1)
			shift_remove(scene, e->obj_type, index);
		return ;
	}
	if (e->action == UNDO_DELETE)
	{
		if (e->obj_type == OBJ_SPHERE)
		{
			scene->sphere_data = realloc(scene->sphere_data,
					sizeof(t_sphere_data)
					* (scene->element_counts.sphere_count + 1));
			scene->sphere_data[scene->element_counts.sphere_count]
				= e->del_sphere;
			scene->element_counts.sphere_count++;
		}
		else if (e->obj_type == OBJ_PLANE)
		{
			scene->plane_data = realloc(scene->plane_data,
					sizeof(t_plane_data)
					* (scene->element_counts.plane_count + 1));
			scene->plane_data[scene->element_counts.plane_count]
				= e->del_plane;
			scene->element_counts.plane_count++;
		}
		else if (e->obj_type == OBJ_CYLINDER)
		{
			scene->cylinder_data = realloc(scene->cylinder_data,
					sizeof(t_cylinder_data)
					* (scene->element_counts.cylinder_count + 1));
			scene->cylinder_data[scene->element_counts.cylinder_count]
				= e->del_cylinder;
			scene->element_counts.cylinder_count++;
		}
		else if (e->obj_type == OBJ_LIGHT)
		{
			scene->light_data = realloc(scene->light_data,
					sizeof(t_light_data)
					* (scene->element_counts.light_count + 1));
			scene->light_data[scene->element_counts.light_count]
				= e->del_light;
			scene->element_counts.light_count++;
		}
		else
		{
			scene->cube_data = realloc(scene->cube_data,
					sizeof(t_cube_data)
					* (scene->element_counts.cube_count + 1));
			scene->cube_data[scene->element_counts.cube_count]
				= e->del_cube;
			scene->element_counts.cube_count++;
		}
		return ;
	}
	if (e->action == UNDO_PROPERTY)
	{
		index = find_index_by_id(scene, e->obj_type, e->obj_id);
		if (index != -1)
			set_property_value(scene, e->obj_type, index, e->field,
				e->old_scalar);
		return ;
	}
	index = find_index_by_id(scene, e->obj_type, e->obj_id);
	if (index == -1)
		return ;
	if (e->action == UNDO_ROTATE)
		set_object_direction(scene, e->obj_type, index, e->old_value);
	else
		set_object_center(scene, e->obj_type, index, e->old_value);
}
