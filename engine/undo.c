/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   undo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 23:12:06 by aaycan            #+#    #+#             */
/*   Updated: 2026/07/17 23:12:12 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	push_undo(t_rt *rt, int type, int index, int is_rotation,
			t_vec3 old_value)
{
	int	i;

	if (rt->undo_count == UNDO_CAPACITY)
	{
		i = 0;
		while (i < UNDO_CAPACITY - 1)
		{
			rt->undo_stack[i] = rt->undo_stack[i + 1];
			i++;
		}
		rt->undo_count--;
	}
	rt->undo_stack[rt->undo_count].obj_type = type;
	rt->undo_stack[rt->undo_count].obj_index = index;
	rt->undo_stack[rt->undo_count].is_rotation = is_rotation;
	rt->undo_stack[rt->undo_count].old_value = old_value;
	rt->undo_count++;
}

void	undo_last(t_rt *rt)
{
	t_undo_entry	*e;

	if (rt->undo_count == 0)
		return ;
	rt->undo_count--;
	e = &rt->undo_stack[rt->undo_count];
	if (e->is_rotation)
		set_object_direction(rt->old_data->scene, e->obj_type,
			e->obj_index, e->old_value);
	else
		set_object_center(rt->old_data->scene, e->obj_type,
			e->obj_index, e->old_value);
}
