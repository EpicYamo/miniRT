/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 22:43:55 by aaycan            #+#    #+#             */
/*   Updated: 2026/02/13 02:47:09 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <stddef.h>

typedef struct s_scene_element_count
{
	size_t	sphere_count;
	size_t	plane_count;
	size_t	cylinder_count;
}	t_element_count;

typedef struct s_ambient_data
{
	int				existence;
	double			ratio;
	unsigned int	red;
	unsigned int	green;
	unsigned int	blue;
}	t_ambient_data;

typedef struct s_camera_data
{
	int		existence;
	double	pos_x;
	double	pos_y;
	double	pos_z;
	double	vector_x;
	double	vector_y;
	double	vector_z;
	int		fov;
}	t_camera_data;

typedef struct s_light_data
{
	int		existence;
	double	pos_x;
	double	pos_y;
	double	pos_z;
	double	brightness;
}	t_light_data;

typedef struct s_sphere_data
{
	double			pos_x;
	double			pos_y;
	double			pos_z;
	double			diameter;
	unsigned int	red;
	unsigned int	green;
	unsigned int	blue;
}	t_sphere_data;

typedef struct s_plane_data
{
	double			pos_x;
	double			pos_y;
	double			pos_z;
	double			vector_x;
	double			vector_y;
	double			vector_z;
	unsigned int	red;
	unsigned int	green;
	unsigned int	blue;
}	t_plane_data;

typedef struct s_cylinder_data
{
	double			pos_x;
	double			pos_y;
	double			pos_z;
	double			vector_x;
	double			vector_y;
	double			vector_z;
	double			diameter;
	double			height;
	unsigned int	red;
	unsigned int	green;
	unsigned int	blue;
}	t_cylinder_data;

typedef struct s_scene
{
	t_ambient_data	ambient_data;
	t_camera_data	camera_data;
	t_light_data	light_data;
	t_element_count	element_counts;
	t_sphere_data	*sphere_data;
	t_plane_data	*plane_data;
	t_cylinder_data	*cylinder_data;
}	t_scene;

typedef struct s_data
{
	t_scene	*scene;
	void	*mlx_ptr;
	void	*mlx_window;
}	t_data;

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}	t_vec3;

char	*read_file(char *file_path);
int		check_file(char *file_path);
void	check_scene(char *file_path);
void	check_param_count(char **scene, char *params, int params_c);
void	validate_ambient_data(char **scene);
void	validate_camera_data(char **scene);
void	check_coordinates(char **scene, char *coords);
void	check_vector_val(char **scene, char *vector);
void	validate_light_data(char **scene);
void	check_normalized_vector(char **scene, char *vector);
void	check_ratio(char **scene, char *ratio);
void	check_colors_range(char **scene, char *range);
void	validate_sphere_data(char **scene);
void	check_diameter_height(char **scene, char *value);
void	validate_plane_data(char **scene);
void	validate_cylinder_data(char **scene);
void	skip_to_next_parameter(char **scene, size_t *i, size_t *j);
t_scene	*parse_scene(char *file_path);
void	fill_colors(unsigned int *red, unsigned int *green,
			unsigned int *blue, char *range);
void	fill_coordinates(double *pos_x, double *pos_y, double *pos_z,
			char *coords);
void	fill_normalized_vector(double *vec_x, double *vec_y, double *vec_z,
			char *vector);
void	create_ambient_data(t_scene *scene, char **scene_map);
void	create_camera_data(t_scene *scene, char **scene_map);
void	create_light_data(t_scene *scene, char **scene_map);
void	create_parameter_count(t_scene *scene, char **scene_map);
void	create_sphere_data(t_scene *scene, char **scene_map);
void	create_plane_data(t_scene *scene, char **scene_map);
void	create_cylinder_data(t_scene *scene, char **scene_map);
void	run_engine(void);

size_t	ft_strlen(const char *s);
void	error_message(int code, char *str);
char	*ft_strjoin(char const *s1, char const *s2);
void	*ft_calloc(size_t count, size_t size);
char	**ft_split(char const *s, char c);
void	free_two_dim_array(char **arr);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_isdigit(int d);
int		ft_atoi(const char *str);
void	free_arr_error_message(char **scene);
void	free_scene(void);
double	ft_atod(const char *s);

void	print_the_scene(t_scene *scene);

#endif
