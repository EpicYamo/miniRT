/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   property_panel.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include "../minilibx-linux/mlx.h"
#include <string.h>
#include <stdio.h>

static int	get_field_list(int type, int *list)
{
	int	n;

	n = 0;
	list[n++] = PROP_COLOR_R;
	list[n++] = PROP_COLOR_G;
	list[n++] = PROP_COLOR_B;
	if (type == OBJ_SPHERE || type == OBJ_CYLINDER || type == OBJ_CUBE)
		list[n++] = PROP_SIZE1;
	if (type == OBJ_LIGHT)
		list[n++] = PROP_SIZE1;
	if (type == OBJ_CYLINDER)
		list[n++] = PROP_SIZE2;
	if (type != OBJ_LIGHT)
	{
		list[n++] = PROP_CHECKER;
		list[n++] = PROP_SHININESS;
		list[n++] = PROP_SPEC_STRENGTH;
	}
	if (type == OBJ_SPHERE)
		list[n++] = PROP_TEXTURE;
	return (n);
}

void	cycle_property_field(t_rt *rt)
{
	int	list[9];
	int	count;
	int	i;
	int	cur_pos;

	if (rt->input.selected_type == OBJ_NONE)
		return ;
	count = get_field_list(rt->input.selected_type, list);
	cur_pos = -1;
	i = 0;
	while (i < count)
	{
		if (list[i] == rt->input.active_property)
			cur_pos = i;
		i++;
	}
	if (cur_pos == -1 || cur_pos == count - 1)
		rt->input.active_property = list[0];
	else
		rt->input.active_property = list[cur_pos + 1];
}

double	get_property_value(t_scene *scene, int type, int index, int field)
{
	if (type == OBJ_SPHERE)
	{
		if (field == PROP_COLOR_R)
			return (scene->sphere_data[index].red);
		if (field == PROP_COLOR_G)
			return (scene->sphere_data[index].green);
		if (field == PROP_COLOR_B)
			return (scene->sphere_data[index].blue);
		if (field == PROP_SIZE1)
			return (scene->sphere_data[index].diameter);
		if (field == PROP_CHECKER)
			return (scene->sphere_data[index].checker);
		if (field == PROP_SHININESS)
			return (scene->sphere_data[index].shininess);
		if (field == PROP_SPEC_STRENGTH)
			return (scene->sphere_data[index].specular_strength);
		return (scene->sphere_data[index].has_texture);
	}
	if (type == OBJ_CYLINDER)
	{
		if (field == PROP_COLOR_R)
			return (scene->cylinder_data[index].red);
		if (field == PROP_COLOR_G)
			return (scene->cylinder_data[index].green);
		if (field == PROP_COLOR_B)
			return (scene->cylinder_data[index].blue);
		if (field == PROP_SIZE1)
			return (scene->cylinder_data[index].diameter);
		if (field == PROP_SIZE2)
			return (scene->cylinder_data[index].height);
		if (field == PROP_CHECKER)
			return (scene->cylinder_data[index].checker);
		if (field == PROP_SHININESS)
			return (scene->cylinder_data[index].shininess);
		return (scene->cylinder_data[index].specular_strength);
	}
	if (type == OBJ_LIGHT)
	{
		if (field == PROP_COLOR_R)
			return (scene->light_data[index].red);
		if (field == PROP_COLOR_G)
			return (scene->light_data[index].green);
		if (field == PROP_COLOR_B)
			return (scene->light_data[index].blue);
		return (scene->light_data[index].diameter);
	}
	if (type == OBJ_CUBE)
	{
		if (field == PROP_COLOR_R)
			return (scene->cube_data[index].red);
		if (field == PROP_COLOR_G)
			return (scene->cube_data[index].green);
		if (field == PROP_COLOR_B)
			return (scene->cube_data[index].blue);
		if (field == PROP_SIZE1)
			return (scene->cube_data[index].size);
		if (field == PROP_CHECKER)
			return (scene->cube_data[index].checker);
		if (field == PROP_SHININESS)
			return (scene->cube_data[index].shininess);
		return (scene->cube_data[index].specular_strength);
	}
	if (field == PROP_COLOR_R)
		return (scene->plane_data[index].red);
	if (field == PROP_COLOR_G)
		return (scene->plane_data[index].green);
	if (field == PROP_COLOR_B)
		return (scene->plane_data[index].blue);
	if (field == PROP_CHECKER)
		return (scene->plane_data[index].checker);
	if (field == PROP_SHININESS)
		return (scene->plane_data[index].shininess);
	return (scene->plane_data[index].specular_strength);
}

static double	clamp_field(int type, int field, double value)
{
	if (field == PROP_COLOR_R || field == PROP_COLOR_G
		|| field == PROP_COLOR_B)
	{
		if (value < 0.0)
			value = 0.0;
		if (value > 255.0)
			value = 255.0;
	}
	else if (field == PROP_CHECKER || field == PROP_TEXTURE)
	{
		if (value >= 0.5)
			value = 1.0;
		else
			value = 0.0;
	}
	else if (field == PROP_SHININESS)
	{
		if (value < 1.0)
			value = 1.0;
	}
	else if (field == PROP_SPEC_STRENGTH)
	{
		if (value < 0.0)
			value = 0.0;
	}
	else if (type == OBJ_LIGHT)
	{
		if (value < 0.0)
			value = 0.0;
	}
	else if (value < 0.1)
		value = 0.1;
	return (value);
}

static void	set_sphere_field(t_scene *scene, int index, int field,
			double value)
{
	if (field == PROP_COLOR_R)
		scene->sphere_data[index].red = (unsigned int)value;
	else if (field == PROP_COLOR_G)
		scene->sphere_data[index].green = (unsigned int)value;
	else if (field == PROP_COLOR_B)
		scene->sphere_data[index].blue = (unsigned int)value;
	else if (field == PROP_SIZE1)
		scene->sphere_data[index].diameter = value;
	else if (field == PROP_CHECKER)
		scene->sphere_data[index].checker = (int)value;
	else if (field == PROP_SHININESS)
		scene->sphere_data[index].shininess = value;
	else if (field == PROP_SPEC_STRENGTH)
		scene->sphere_data[index].specular_strength = value;
	else
		scene->sphere_data[index].has_texture = (int)value;
}

static void	set_cylinder_field(t_scene *scene, int index, int field,
			double value)
{
	if (field == PROP_COLOR_R)
		scene->cylinder_data[index].red = (unsigned int)value;
	else if (field == PROP_COLOR_G)
		scene->cylinder_data[index].green = (unsigned int)value;
	else if (field == PROP_COLOR_B)
		scene->cylinder_data[index].blue = (unsigned int)value;
	else if (field == PROP_SIZE1)
		scene->cylinder_data[index].diameter = value;
	else if (field == PROP_SIZE2)
		scene->cylinder_data[index].height = value;
	else if (field == PROP_CHECKER)
		scene->cylinder_data[index].checker = (int)value;
	else if (field == PROP_SHININESS)
		scene->cylinder_data[index].shininess = value;
	else
		scene->cylinder_data[index].specular_strength = value;
}

static void	set_cube_field(t_scene *scene, int index, int field,
			double value)
{
	if (field == PROP_COLOR_R)
		scene->cube_data[index].red = (unsigned int)value;
	else if (field == PROP_COLOR_G)
		scene->cube_data[index].green = (unsigned int)value;
	else if (field == PROP_COLOR_B)
		scene->cube_data[index].blue = (unsigned int)value;
	else if (field == PROP_SIZE1)
		scene->cube_data[index].size = value;
	else if (field == PROP_CHECKER)
		scene->cube_data[index].checker = (int)value;
	else if (field == PROP_SHININESS)
		scene->cube_data[index].shininess = value;
	else
		scene->cube_data[index].specular_strength = value;
}

static void	set_plane_field(t_scene *scene, int index, int field,
			double value)
{
	if (field == PROP_COLOR_R)
		scene->plane_data[index].red = (unsigned int)value;
	else if (field == PROP_COLOR_G)
		scene->plane_data[index].green = (unsigned int)value;
	else if (field == PROP_COLOR_B)
		scene->plane_data[index].blue = (unsigned int)value;
	else if (field == PROP_CHECKER)
		scene->plane_data[index].checker = (int)value;
	else if (field == PROP_SHININESS)
		scene->plane_data[index].shininess = value;
	else
		scene->plane_data[index].specular_strength = value;
}

void	set_property_value(t_scene *scene, int type, int index, int field,
			double value)
{
	value = clamp_field(type, field, value);
	if (type == OBJ_SPHERE)
		set_sphere_field(scene, index, field, value);
	else if (type == OBJ_CYLINDER)
		set_cylinder_field(scene, index, field, value);
	else if (type == OBJ_CUBE)
		set_cube_field(scene, index, field, value);
	else if (type == OBJ_LIGHT)
	{
		if (field == PROP_COLOR_R)
			scene->light_data[index].red = (unsigned int)value;
		else if (field == PROP_COLOR_G)
			scene->light_data[index].green = (unsigned int)value;
		else if (field == PROP_COLOR_B)
			scene->light_data[index].blue = (unsigned int)value;
		else
			scene->light_data[index].diameter = value;
	}
	else
		set_plane_field(scene, index, field, value);
}

static void	append_field_line(char *buf, char *label, double value,
			int field, int active)
{
	char	num[32];

	strcat(buf, label);
	strcat(buf, ": ");
	sprintf(num, "%.2f", value);
	strcat(buf, num);
	if (field == active)
		strcat(buf, "  <--");
	strcat(buf, "   ");
}

static char	*size_label(int type)
{
	if (type == OBJ_CUBE)
		return ("Size");
	return ("Diameter");
}

void	draw_property_panel(t_rt *rt)
{
	int		index;
	int		type;
	char	line1[128];
	char	line2[160];
	int		y;
	t_scene	*scene;

	type = rt->input.selected_type;
	if (type == OBJ_NONE)
		return ;
	scene = rt->old_data->scene;
	index = find_index_by_id(scene, type, rt->input.selected_id);
	if (index == -1)
		return ;
	y = 60;
	line1[0] = '\0';
	append_field_line(line1, "R", get_property_value(scene, type, index,
				PROP_COLOR_R), PROP_COLOR_R, rt->input.active_property);
	append_field_line(line1, "G", get_property_value(scene, type, index,
				PROP_COLOR_G), PROP_COLOR_G, rt->input.active_property);
	append_field_line(line1, "B", get_property_value(scene, type, index,
				PROP_COLOR_B), PROP_COLOR_B, rt->input.active_property);
	mlx_string_put(rt->old_data->mlx_ptr, rt->old_data->mlx_window,
		20, y, 0xFFFFFF, line1);
	line2[0] = '\0';
	if (type == OBJ_SPHERE || type == OBJ_CYLINDER || type == OBJ_LIGHT
		|| type == OBJ_CUBE)
		append_field_line(line2, size_label(type), get_property_value(
					scene, type, index, PROP_SIZE1), PROP_SIZE1,
				rt->input.active_property);
	if (type == OBJ_CYLINDER)
		append_field_line(line2, "Height", get_property_value(scene,
					type, index, PROP_SIZE2), PROP_SIZE2,
				rt->input.active_property);
	if (line2[0] != '\0')
		mlx_string_put(rt->old_data->mlx_ptr, rt->old_data->mlx_window,
			20, y + 40, 0xFFFFFF, line2);
	if (type == OBJ_LIGHT)
		return ;
	line2[0] = '\0';
	append_field_line(line2, "Checker", get_property_value(scene, type,
				index, PROP_CHECKER), PROP_CHECKER,
			rt->input.active_property);
	append_field_line(line2, "Shininess", get_property_value(scene, type,
				index, PROP_SHININESS), PROP_SHININESS,
			rt->input.active_property);
	append_field_line(line2, "Specular", get_property_value(scene, type,
				index, PROP_SPEC_STRENGTH), PROP_SPEC_STRENGTH,
			rt->input.active_property);
	if (type == OBJ_SPHERE)
		append_field_line(line2, "Texture", get_property_value(scene,
					type, index, PROP_TEXTURE), PROP_TEXTURE,
				rt->input.active_property);
	mlx_string_put(rt->old_data->mlx_ptr, rt->old_data->mlx_window,
		20, y + 80, 0xFFFFFF, line2);
}
