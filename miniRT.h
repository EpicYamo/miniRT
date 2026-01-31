/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 22:43:55 by aaycan            #+#    #+#             */
/*   Updated: 2026/01/31 18:17:42 by aaycan           ###   ########.fr       */
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
	unsigned int	ratio;
	unsigned int	red;
	unsigned int	green;
	unsigned int	blue;
}	t_ambient_data;

typedef struct s_camera_data
{
	int			existence;
	long long	pos_x;
	long long	pos_y;
	long long	pos_z;
	int			vector_x;
	int			vector_y;
	int			vector_z;
	int			fov;
}	t_camera_data;

typedef struct s_light_data
{
	int				existence;
	long long		pos_x;
	long long		pos_y;
	long long		pos_z;
	unsigned int	brigthness;
}	t_light_data;

typedef struct s_sphere_data
{
	long long		pos_x;
	long long		pos_y;
	long long		pos_z;
	unsigned int	diameter;
	unsigned int	red;
	unsigned int	green;
	unsigned int	blue;
}	t_sphere_data;

typedef struct s_plane_data
{
	long long		pos_x;
	long long		pos_y;
	long long		pos_z;
	int				vector_x;
	int				vector_y;
	int				vector_z;
	unsigned int	red;
	unsigned int	green;
	unsigned int	blue;
}	t_plane_data;

typedef struct s_cylinder_data
{
	long long		pos_x;
	long long		pos_y;
	long long		pos_z;
	int				vector_x;
	int				vector_y;
	int				vector_z;
	unsigned int	diameter;
	unsigned int	height;
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
    double x;
    double y;
    double z;
} t_vec3;

char	*read_file(char *file_path);
int		check_file(char *file_path);
void	check_scene(char *file_path);
void	check_param_count(char **scene, char *params, int params_c);
void	validate_ambient_data(char **scene);
void	validate_camera_data(char **scene);
void	check_coordinates(char **scene, char *coords);
void	check_vector_val(char **scene, char *vector);
void	validate_light_data(char **scene);
void	check_ratio(char **scene, char *ratio);
void	check_colors_range(char **scene, char *range);
void	validate_sphere_data(char **scene);
void	check_diameter_height(char **scene, char *value);
void	validate_plane_data(char **scene);
void	validate_cylinder_data(char **scene);
void	skip_to_next_parameter(char **scene, size_t *i, size_t *j);
t_scene	*parse_scene(char *file_path);
void	fill_ambient_ratio(t_scene *scene, char *ratio);
void	fill_ambient_colors(t_scene *scene, char *range);
void	fill_camera_coordinates(t_scene *scene, char *coords);
void	fill_camera_orientation_vector(t_scene *scene, char *vector);
void	fill_blank(t_scene *scene);
void	create_light_data(t_scene *scene, char **scene_map);
void	fill_light_coordinates(t_scene *scene, char *coords);
void	create_parameter_count(t_scene *scene, char **scene_map);
void	create_sphere_data(t_scene *scene, char **scene_map);
void	increment_index_to_the_next_param(char *scene_map, size_t *j);
void	fill_sphere_coordinates(t_scene *scene, char *coords, int index);
void	fill_sphere_diameter(t_scene *scene, char *diameter, int index);
void	fill_sphere_colors(t_scene *scene, char *range, int index);
void	create_plane_data(t_scene *scene, char **scene_map);
void	fill_plane_coordinates(t_scene *scene, char *coords, int index);
void	fill_plane_vector(t_scene *scene, char *vector, int index);
void	create_cylinder_data(t_scene *scene, char **scene_map);
void	fill_cylinder_coordinates(t_scene *scene, char *coords, int index);
void	fill_cylinder_vector(t_scene *scene, char *vector, int index);
void	fill_cylinder_diameter(t_scene *scene, char *diameter, int index);
void	fill_cylinder_height(t_scene *scene, char *height, int index);
void	fill_cylinder_colors(t_scene *scene, char *range, int index);
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

void	print_the_scene(t_scene *scene);

#endif
