/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_scene.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 13:40:05 by aaycan            #+#    #+#             */
/*   Updated: 2025/11/04 14:34:30 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include <stdio.h>

static void	print_sphere_data(t_scene *scene);
static void	print_plane_data(t_scene *scene);
static void	print_cylinder_data(t_scene *scene);

void	print_the_scene(t_scene *scene)
{
	printf("\nAmbient Light\nExistence: %d\n", scene->ambient_data.existence);
	printf("raio: %d\n", scene->ambient_data.ratio);
	printf("color_red: %d\ncolor_green: %d\ncolor_blue: %d\n\n",
		scene->ambient_data.red, scene->ambient_data.green,
		scene->ambient_data.blue);
	printf("Camera\nExistence: %d\n", scene->camera_data.existence);
	printf("pos_x: %lld\npos_y: %lld\npos_z: %lld\n",
		scene->camera_data.pos_x, scene->camera_data.pos_y,
		scene->camera_data.pos_z);
	printf("v_pos_x: %d\nv_pos_y: %d\nv_pos_z: %d\n",
		scene->camera_data.vector_x, scene->camera_data.vector_y,
		scene->camera_data.vector_z);
	printf("fov: %d\n\n", scene->camera_data.fov);
	printf("Light\nExistence: %d\n", scene->light_data.existence);
	printf("pos_x: %lld\npos_y: %lld\npos_z: %lld\n",
		scene->light_data.pos_x, scene->light_data.pos_y,
		scene->light_data.pos_z);
	printf("brightness_ratio: %d\n\n", scene->light_data.brigthness);
	print_sphere_data(scene);
	print_plane_data(scene);
	print_cylinder_data(scene);
}

static void	print_sphere_data(t_scene *scene)
{
	size_t	i;

	printf("Sphere\nSphere Count: %ld\n", scene->element_counts.sphere_count);
	if ((scene->element_counts.sphere_count == 0)
		|| (scene->sphere_data == NULL))
	{
		printf("\n");
		return ;
	}
	i = 0;
	while (i < scene->element_counts.sphere_count)
	{
		printf("sphere_id: %ld\n", i);
		printf("pos_x: %lld\npos_y: %lld\npos_z: %lld\n",
			scene->sphere_data[i].pos_x, scene->sphere_data[i].pos_y,
			scene->sphere_data[i].pos_z);
		printf("diameter: %d\n", scene->sphere_data[i].diameter);
		printf("color_red: %d\ncolor_green: %d\ncolor_blue: %d\n\n",
			scene->sphere_data[i].red, scene->sphere_data[i].green,
			scene->sphere_data[i].blue);
		i++;
	}
}

static void	print_plane_data(t_scene *scene)
{
	size_t	i;

	printf("Plane\nPlane Count: %ld\n", scene->element_counts.plane_count);
	if ((scene->element_counts.plane_count == 0) || (scene->plane_data == NULL))
	{
		printf("\n");
		return ;
	}
	i = 0;
	while (i < scene->element_counts.plane_count)
	{
		printf("plane_id: %ld\n", i);
		printf("pos_x: %lld\npos_y: %lld\npos_z: %lld\n",
			scene->plane_data[i].pos_x, scene->plane_data[i].pos_y,
			scene->plane_data[i].pos_z);
		printf("v_pos_x: %d\nv_pos_y: %d\nv_pos_z: %d\n",
			scene->plane_data[i].vector_x, scene->plane_data[i].vector_y,
			scene->plane_data[i].vector_z);
		printf("color_red: %d\ncolor_green: %d\ncolor_blue: %d\n\n",
			scene->plane_data[i].red, scene->plane_data[i].green,
			scene->plane_data[i].blue);
		i++;
	}
}

static void	print_cylinder_data(t_scene *scene)
{
	size_t	i;

	printf("Cylinder\nCylinder Count: %ld\n",
		scene->element_counts.cylinder_count);
	if ((scene->element_counts.cylinder_count == 0)
		|| (scene->cylinder_data == NULL))
		return ;
	i = 0;
	while (i < scene->element_counts.cylinder_count)
	{
		printf("cylinder_id: %ld\n", i);
		printf("pos_x: %lld\npos_y: %lld\npos_z: %lld\n",
			scene->cylinder_data[i].pos_x, scene->cylinder_data[i].pos_y,
			scene->cylinder_data[i].pos_z);
		printf("v_pos_x: %d\nv_pos_y: %d\nv_pos_z: %d\n",
			scene->cylinder_data[i].vector_x, scene->cylinder_data[i].vector_y,
			scene->cylinder_data[i].vector_z);
		printf("diameter: %d\n", scene->cylinder_data[i].diameter);
		printf("height: %d\n", scene->cylinder_data[i].height);
		printf("color_red: %d\ncolor_green: %d\ncolor_blue: %d\n\n",
			scene->cylinder_data[i].red, scene->cylinder_data[i].green,
			scene->cylinder_data[i].blue);
		i++;
	}
}
