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

static int	max_field_for_type(int type)
{
	if (type == OBJ_SPHERE || type == OBJ_LIGHT || type == OBJ_CUBE)
		return (PROP_SIZE1);
	if (type == OBJ_CYLINDER)
		return (PROP_SIZE2);
	return (PROP_COLOR_B);
}

void	cycle_property_field(t_rt *rt)
{
	int	max_field;

	if (rt->input.selected_type == OBJ_NONE)
		return ;
	max_field = max_field_for_type(rt->input.selected_type);
	rt->input.active_property++;
	if (rt->input.active_property > max_field)
		rt->input.active_property = PROP_COLOR_R;
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
		return (scene->sphere_data[index].diameter);
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
		return (scene->cylinder_data[index].height);
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
		return (scene->cube_data[index].size);
	}
	if (field == PROP_COLOR_R)
		return (scene->plane_data[index].red);
	if (field == PROP_COLOR_G)
		return (scene->plane_data[index].green);
	return (scene->plane_data[index].blue);
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
	else if (type == OBJ_LIGHT)
	{
		if (value < 0.0)
			value = 0.0;
	}
	else if (value < 0.1)
		value = 0.1;
	return (value);
}

void	set_property_value(t_scene *scene, int type, int index, int field,
			double value)
{
	value = clamp_field(type, field, value);
	if (type == OBJ_SPHERE)
	{
		if (field == PROP_COLOR_R)
			scene->sphere_data[index].red = (unsigned int)value;
		else if (field == PROP_COLOR_G)
			scene->sphere_data[index].green = (unsigned int)value;
		else if (field == PROP_COLOR_B)
			scene->sphere_data[index].blue = (unsigned int)value;
		else
			scene->sphere_data[index].diameter = value;
	}
	else if (type == OBJ_CYLINDER)
	{
		if (field == PROP_COLOR_R)
			scene->cylinder_data[index].red = (unsigned int)value;
		else if (field == PROP_COLOR_G)
			scene->cylinder_data[index].green = (unsigned int)value;
		else if (field == PROP_COLOR_B)
			scene->cylinder_data[index].blue = (unsigned int)value;
		else if (field == PROP_SIZE1)
			scene->cylinder_data[index].diameter = value;
		else
			scene->cylinder_data[index].height = value;
	}
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
	else if (type == OBJ_CUBE)
	{
		if (field == PROP_COLOR_R)
			scene->cube_data[index].red = (unsigned int)value;
		else if (field == PROP_COLOR_G)
			scene->cube_data[index].green = (unsigned int)value;
		else if (field == PROP_COLOR_B)
			scene->cube_data[index].blue = (unsigned int)value;
		else
			scene->cube_data[index].size = value;
	}
	else
	{
		if (field == PROP_COLOR_R)
			scene->plane_data[index].red = (unsigned int)value;
		else if (field == PROP_COLOR_G)
			scene->plane_data[index].green = (unsigned int)value;
		else
			scene->plane_data[index].blue = (unsigned int)value;
	}
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

void	draw_property_panel(t_rt *rt)
{
	int		index;
	int		type;
	char	line1[128];
	char	line2[128];
	int		y;

	type = rt->input.selected_type;
	if (type == OBJ_NONE)
		return ;
	index = find_index_by_id(rt->old_data->scene, type, rt->input.selected_id);
	if (index == -1)
		return ;
	y = 60;
	line1[0] = '\0';
	append_field_line(line1, "R", get_property_value(rt->old_data->scene,
				type, index, PROP_COLOR_R), PROP_COLOR_R,
			rt->input.active_property);
	append_field_line(line1, "G", get_property_value(rt->old_data->scene,
				type, index, PROP_COLOR_G), PROP_COLOR_G,
			rt->input.active_property);
	append_field_line(line1, "B", get_property_value(rt->old_data->scene,
				type, index, PROP_COLOR_B), PROP_COLOR_B,
			rt->input.active_property);
	mlx_string_put(rt->old_data->mlx_ptr, rt->old_data->mlx_window,
		20, y, 0xFFFFFF, line1);
	line2[0] = '\0';
	if (type == OBJ_SPHERE || type == OBJ_CYLINDER || type == OBJ_LIGHT)
		append_field_line(line2, "Diameter", get_property_value(
					rt->old_data->scene, type, index, PROP_SIZE1),
				PROP_SIZE1, rt->input.active_property);
	else if (type == OBJ_CUBE)
		append_field_line(line2, "Size", get_property_value(
					rt->old_data->scene, type, index, PROP_SIZE1),
				PROP_SIZE1, rt->input.active_property);
	if (type == OBJ_CYLINDER)
		append_field_line(line2, "Height", get_property_value(
					rt->old_data->scene, type, index, PROP_SIZE2),
				PROP_SIZE2, rt->input.active_property);
	if (line2[0] != '\0')
		mlx_string_put(rt->old_data->mlx_ptr, rt->old_data->mlx_window,
			20, y + 40, 0xFFFFFF, line2);
}
