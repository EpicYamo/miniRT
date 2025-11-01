/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_verifier_pt_four.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 16:37:44 by aaycan            #+#    #+#             */
/*   Updated: 2025/11/01 10:45:22 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

static void	if_vector_val_single_digit(char **scene, char *vector);
static void	if_vector_val_double_digit(char **scene, char *vector);
static void	if_vector_val_three_digit(char **scene, char *vector);
static void	if_vector_val_four_digit(char **scene, char *vector);

void	check_vector_val(char **scene, char *vector)
{
	size_t	i;
	size_t	dot_count;

	if ((vector[0] == ',') || (vector[0] == '.'))
		free_arr_error_message(scene);
	i = 0;
	dot_count = 0;
	while ((vector[i]) && (vector[i] != ',') && (vector[i] != ' '))
	{
		if (vector[i] == '.')
			dot_count += 1;
		i++;
	}
	if (dot_count > 1)
		free_arr_error_message(scene);
	if (i == 1)
		if_vector_val_single_digit(scene, vector);
	else if (i == 2)
		if_vector_val_double_digit(scene, vector);
	else if (i == 3)
		if_vector_val_three_digit(scene, vector);
	else if (i == 4)
		if_vector_val_four_digit(scene, vector);
	else
		free_arr_error_message(scene);
}

static void	if_vector_val_single_digit(char **scene, char *vector)
{
	if ((vector[0] != '0') && (vector[0] != '1'))
		free_arr_error_message(scene);
}

static void	if_vector_val_double_digit(char **scene, char *vector)
{
	if (((vector[0] != '-') && (vector[1] != '1'))
		|| ((vector[0] != '-') && (vector[1] != '0')))
		free_arr_error_message(scene);
}

static void	if_vector_val_three_digit(char **scene, char *vector)
{
	if (((vector[0] != '0') && (vector[0] != '1')) || (vector[1] != '.'))
		free_arr_error_message(scene);
	if ((vector[0] == '1') && (vector[2] != '0'))
		free_arr_error_message(scene);
	if (!ft_isdigit(vector[2]))
		free_arr_error_message(scene);
}

static void	if_vector_val_four_digit(char **scene, char *vector)
{
	if (vector[0] != '-')
		free_arr_error_message(scene);
	if (((vector[1] != '0') && (vector[1] != '1')) || (vector[2] != '.'))
		free_arr_error_message(scene);
	if ((vector[1] == '1') && (vector[3] != '0'))
		free_arr_error_message(scene);
	if (!ft_isdigit(vector[3]))
		free_arr_error_message(scene);
}
