/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spawn.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 00:20:44 by aaycan            #+#    #+#             */
/*   Updated: 2026/07/18 00:53:36 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include <stdlib.h>

static t_vec3	spawn_position(t_scene *scene)
{
	t_vec3	pos;
	t_vec3	forward;

	pos = vec3_create(scene->camera_data.pos_x,
			scene->camera_data.pos_y, scene->camera_data.pos_z);
	forward = vec3_normalize(vec3_create(scene->camera_data.vector_x,
				scene->camera_data.vector_y, scene->camera_data.vector_z));
	return (vec3_add(pos, vec3_mul(forward, 30.0)));
}

void	spawn_sphere(t_rt *rt)
{
	t_scene			*scene;
	t_sphere_data	*new_arr;
	size_t			count;
	t_vec3			pos;

	scene = rt->old_data->scene;
	count = scene->element_counts.sphere_count;
	new_arr = realloc(scene->sphere_data, sizeof(t_sphere_data) * (count + 1));
	if (!new_arr)
		return ;
	scene->sphere_data = new_arr;
	pos = spawn_position(scene);
	scene->sphere_data[count].pos_x = pos.x;
	scene->sphere_data[count].pos_y = pos.y;
	scene->sphere_data[count].pos_z = pos.z;
	scene->sphere_data[count].diameter = 10.0;
	scene->sphere_data[count].red = 255;
	scene->sphere_data[count].green = 255;
	scene->sphere_data[count].blue = 255;
	scene->sphere_data[count].id = rt->input.next_sphere_id++;
	scene->sphere_data[count].checker = 0;
	scene->sphere_data[count].shininess = DEFAULT_SHININESS;
	scene->sphere_data[count].specular_strength = DEFAULT_SPEC_STRENGTH;
	scene->sphere_data[count].has_texture = 0;
	scene->element_counts.sphere_count++;
	rt->input.selected_type = OBJ_SPHERE;
	rt->input.selected_id = scene->sphere_data[count].id;
	rt->input.edit_mode = EDIT_MOVE;
	rt->input.dragging_axis = -1;
	rt->input.active_property = PROP_NONE;
	push_undo(rt, OBJ_SPHERE, scene->sphere_data[count].id, UNDO_SPAWN,
		vec3_create(0.0, 0.0, 0.0));
}

void	spawn_plane(t_rt *rt)
{
	t_scene			*scene;
	t_plane_data	*new_arr;
	size_t			count;
	t_vec3			pos;

	scene = rt->old_data->scene;
	count = scene->element_counts.plane_count;
	new_arr = realloc(scene->plane_data, sizeof(t_plane_data) * (count + 1));
	if (!new_arr)
		return ;
	scene->plane_data = new_arr;
	pos = spawn_position(scene);
	scene->plane_data[count].pos_x = pos.x;
	scene->plane_data[count].pos_y = pos.y;
	scene->plane_data[count].pos_z = pos.z;
	scene->plane_data[count].vector_x = 0.0;
	scene->plane_data[count].vector_y = 1.0;
	scene->plane_data[count].vector_z = 0.0;
	scene->plane_data[count].red = 180;
	scene->plane_data[count].green = 180;
	scene->plane_data[count].blue = 180;
	scene->plane_data[count].id = rt->input.next_plane_id++;
	scene->plane_data[count].checker = 0;
	scene->plane_data[count].shininess = DEFAULT_SHININESS;
	scene->plane_data[count].specular_strength = DEFAULT_SPEC_STRENGTH;
	scene->element_counts.plane_count++;
	rt->input.selected_type = OBJ_PLANE;
	rt->input.selected_id = scene->plane_data[count].id;
	rt->input.edit_mode = EDIT_MOVE;
	rt->input.dragging_axis = -1;
	rt->input.active_property = PROP_NONE;
	push_undo(rt, OBJ_PLANE, scene->plane_data[count].id, UNDO_SPAWN,
		vec3_create(0.0, 0.0, 0.0));
}

void	spawn_cylinder(t_rt *rt)
{
	t_scene			*scene;
	t_cylinder_data	*new_arr;
	size_t			count;
	t_vec3			pos;

	scene = rt->old_data->scene;
	count = scene->element_counts.cylinder_count;
	new_arr = realloc(scene->cylinder_data,
			sizeof(t_cylinder_data) * (count + 1));
	if (!new_arr)
		return ;
	scene->cylinder_data = new_arr;
	pos = spawn_position(scene);
	scene->cylinder_data[count].pos_x = pos.x;
	scene->cylinder_data[count].pos_y = pos.y;
	scene->cylinder_data[count].pos_z = pos.z;
	scene->cylinder_data[count].vector_x = 0.0;
	scene->cylinder_data[count].vector_y = 1.0;
	scene->cylinder_data[count].vector_z = 0.0;
	scene->cylinder_data[count].diameter = 10.0;
	scene->cylinder_data[count].height = 20.0;
	scene->cylinder_data[count].red = 255;
	scene->cylinder_data[count].green = 255;
	scene->cylinder_data[count].blue = 255;
	scene->cylinder_data[count].id = rt->input.next_cylinder_id++;
	scene->cylinder_data[count].checker = 0;
	scene->cylinder_data[count].shininess = DEFAULT_SHININESS;
	scene->cylinder_data[count].specular_strength = DEFAULT_SPEC_STRENGTH;
	scene->element_counts.cylinder_count++;
	rt->input.selected_type = OBJ_CYLINDER;
	rt->input.selected_id = scene->cylinder_data[count].id;
	rt->input.edit_mode = EDIT_MOVE;
	rt->input.dragging_axis = -1;
	rt->input.active_property = PROP_NONE;
	push_undo(rt, OBJ_CYLINDER, scene->cylinder_data[count].id, UNDO_SPAWN,
		vec3_create(0.0, 0.0, 0.0));
}

void	spawn_light(t_rt *rt)
{
	t_scene			*scene;
	t_light_data	*new_arr;
	size_t			count;
	t_vec3			pos;

	scene = rt->old_data->scene;
	count = scene->element_counts.light_count;
	new_arr = realloc(scene->light_data, sizeof(t_light_data) * (count + 1));
	if (!new_arr)
		return ;
	scene->light_data = new_arr;
	pos = spawn_position(scene);
	scene->light_data[count].pos_x = pos.x;
	scene->light_data[count].pos_y = pos.y;
	scene->light_data[count].pos_z = pos.z;
	scene->light_data[count].brightness = LIGHT_SPAWN_BRIGHTNESS;
	scene->light_data[count].red = 255;
	scene->light_data[count].green = 255;
	scene->light_data[count].blue = 255;
	scene->light_data[count].diameter = LIGHT_SPAWN_DIAMETER;
	scene->light_data[count].id = rt->input.next_light_id++;
	scene->element_counts.light_count++;
	rt->input.selected_type = OBJ_LIGHT;
	rt->input.selected_id = scene->light_data[count].id;
	rt->input.edit_mode = EDIT_MOVE;
	rt->input.dragging_axis = -1;
	rt->input.active_property = PROP_NONE;
	push_undo(rt, OBJ_LIGHT, scene->light_data[count].id, UNDO_SPAWN,
		vec3_create(0.0, 0.0, 0.0));
}

void	spawn_cube(t_rt *rt)
{
	t_scene			*scene;
	t_cube_data		*new_arr;
	size_t			count;
	t_vec3			pos;

	scene = rt->old_data->scene;
	count = scene->element_counts.cube_count;
	new_arr = realloc(scene->cube_data, sizeof(t_cube_data) * (count + 1));
	if (!new_arr)
		return ;
	scene->cube_data = new_arr;
	pos = spawn_position(scene);
	scene->cube_data[count].pos_x = pos.x;
	scene->cube_data[count].pos_y = pos.y;
	scene->cube_data[count].pos_z = pos.z;
	scene->cube_data[count].vector_x = 0.0;
	scene->cube_data[count].vector_y = 1.0;
	scene->cube_data[count].vector_z = 0.0;
	scene->cube_data[count].size = CUBE_SPAWN_SIZE;
	scene->cube_data[count].red = 255;
	scene->cube_data[count].green = 255;
	scene->cube_data[count].blue = 255;
	scene->cube_data[count].id = rt->input.next_cube_id++;
	scene->cube_data[count].checker = 0;
	scene->cube_data[count].shininess = DEFAULT_SHININESS;
	scene->cube_data[count].specular_strength = DEFAULT_SPEC_STRENGTH;
	scene->element_counts.cube_count++;
	rt->input.selected_type = OBJ_CUBE;
	rt->input.selected_id = scene->cube_data[count].id;
	rt->input.edit_mode = EDIT_MOVE;
	rt->input.dragging_axis = -1;
	rt->input.active_property = PROP_NONE;
	push_undo(rt, OBJ_CUBE, scene->cube_data[count].id, UNDO_SPAWN,
		vec3_create(0.0, 0.0, 0.0));
}

void	shift_remove(t_scene *scene, int type, int index)
{
	size_t	i;
	size_t	count;

	if (type == OBJ_SPHERE)
	{
		count = scene->element_counts.sphere_count;
		i = index;
		while (i < count - 1)
		{
			scene->sphere_data[i] = scene->sphere_data[i + 1];
			i++;
		}
		scene->element_counts.sphere_count--;
	}
	else if (type == OBJ_PLANE)
	{
		count = scene->element_counts.plane_count;
		i = index;
		while (i < count - 1)
		{
			scene->plane_data[i] = scene->plane_data[i + 1];
			i++;
		}
		scene->element_counts.plane_count--;
	}
	else if (type == OBJ_CYLINDER)
	{
		count = scene->element_counts.cylinder_count;
		i = index;
		while (i < count - 1)
		{
			scene->cylinder_data[i] = scene->cylinder_data[i + 1];
			i++;
		}
		scene->element_counts.cylinder_count--;
	}
	else if (type == OBJ_LIGHT)
	{
		count = scene->element_counts.light_count;
		i = index;
		while (i < count - 1)
		{
			scene->light_data[i] = scene->light_data[i + 1];
			i++;
		}
		scene->element_counts.light_count--;
	}
	else if (type == OBJ_CUBE)
	{
		count = scene->element_counts.cube_count;
		i = index;
		while (i < count - 1)
		{
			scene->cube_data[i] = scene->cube_data[i + 1];
			i++;
		}
		scene->element_counts.cube_count--;
	}
}

void	shift_undo_stack(t_rt *rt)
{
	int	i;

	i = 0;
	while (i < UNDO_CAPACITY - 1)
	{
		rt->undo_stack[i] = rt->undo_stack[i + 1];
		i++;
	}
	rt->undo_count--;
}

void	delete_selected(t_rt *rt)
{
	t_scene			*scene;
	int				type;
	int				id;
	int				index;
	t_undo_entry	*e;

	type = rt->input.selected_type;
	id = rt->input.selected_id;
	if (type == OBJ_NONE)
		return ;
	scene = rt->old_data->scene;
	index = find_index_by_id(scene, type, id);
	if (index == -1)
		return ;
	if (rt->undo_count == UNDO_CAPACITY)
		shift_undo_stack(rt);
	e = &rt->undo_stack[rt->undo_count];
	e->action = UNDO_DELETE;
	e->obj_type = type;
	e->obj_id = id;
	if (type == OBJ_SPHERE)
		e->del_sphere = scene->sphere_data[index];
	else if (type == OBJ_PLANE)
		e->del_plane = scene->plane_data[index];
	else if (type == OBJ_CYLINDER)
		e->del_cylinder = scene->cylinder_data[index];
	else if (type == OBJ_LIGHT)
		e->del_light = scene->light_data[index];
	else
		e->del_cube = scene->cube_data[index];
	rt->undo_count++;
	shift_remove(scene, type, index);
	rt->input.selected_type = OBJ_NONE;
	rt->input.selected_id = -1;
	rt->input.edit_mode = EDIT_NONE;
	rt->input.dragging_axis = -1;
	rt->input.active_property = PROP_NONE;
}
