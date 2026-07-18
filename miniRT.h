/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 22:43:55 by aaycan            #+#    #+#             */
/*   Updated: 2026/07/18 04:19:00 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# define WIDTH 2560
# define HEIGHT 1440
# define M_PI 3.14159265358979323846
# define EPSILON 1e-4
# define MOUSE_SENSITIVITY 0.0015
# define PITCH_LIMIT (89.0 * M_PI / 180.0)
# define MODE_RENDER 0
# define MODE_NAVIGATE 1
# define NAV_RENDER_STEP 8
# define MOVE_SPEED 40.0
# define LIGHT_MARKER_DIAMETER 2.0
# define OBJ_NONE -1
# define OBJ_SPHERE 0
# define OBJ_PLANE 1
# define OBJ_CYLINDER 2
# define OBJ_LIGHT 3
# define OBJ_CUBE 4
# define NAV_LIGHT_LIMIT 2
# define LIGHT_SPAWN_DIAMETER 2.0
# define LIGHT_SPAWN_BRIGHTNESS 0.6
# define CUBE_SPAWN_SIZE 12.0
# define OUTLINE_THICKNESS 2
# define PLANE_MARKER_RADIUS 15.0
# define EDIT_NONE 0
# define EDIT_MOVE 1
# define EDIT_ROTATE 2
# define GIZMO_LENGTH 15.0
# define AXIS_PICK_RADIUS (8.0 + GIZMO_LINE_THICKNESS * 2.0)
# define GIZMO_LINE_THICKNESS 4
# define ROTATE_SENSITIVITY 0.01
# define RING_SEGMENTS 32
# define UNDO_CAPACITY 50
# define TEXT_BUFFER_SIZE 32
# define UNDO_MOVE 0
# define UNDO_ROTATE 1
# define UNDO_DELETE 2
# define UNDO_SPAWN 3
# define UNDO_PROPERTY 4
# define PROP_NONE -1
# define PROP_COLOR_R 0
# define PROP_COLOR_G 1
# define PROP_COLOR_B 2
# define PROP_SIZE1 3
# define PROP_SIZE2 4
# define PROP_CHECKER 5
# define PROP_SHININESS 6
# define PROP_SPEC_STRENGTH 7
# define PROP_TEXTURE 8
# define CHECKER_CELL_SIZE 10.0
# define DEFAULT_SHININESS 32.0
# define DEFAULT_SPEC_STRENGTH 0.6
# define TEXTURE_PATH "textures/sphere_texture.xpm"
# define AA_SAMPLES 2
# define TEXT_TARGET_AXIS 0
# define TEXT_TARGET_PROPERTY 1

# include <stddef.h>

typedef struct s_scene_element_count
{
	size_t	sphere_count;
	size_t	plane_count;
	size_t	cylinder_count;
	size_t	light_count;
	size_t	cube_count;
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
	double			pos_x;
	double			pos_y;
	double			pos_z;
	double			brightness;
	unsigned int	red;
	unsigned int	green;
	unsigned int	blue;
	double			diameter;
	int				id;
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
	int				id;
	int				checker;
	double			shininess;
	double			specular_strength;
	int				has_texture;
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
	int				id;
	int				checker;
	double			shininess;
	double			specular_strength;
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
	int				id;
	int				checker;
	double			shininess;
	double			specular_strength;
}	t_cylinder_data;

typedef struct s_cube_data
{
	double			pos_x;
	double			pos_y;
	double			pos_z;
	double			vector_x;
	double			vector_y;
	double			vector_z;
	double			size;
	unsigned int	red;
	unsigned int	green;
	unsigned int	blue;
	int				id;
	int				checker;
	double			shininess;
	double			specular_strength;
}	t_cube_data;

typedef struct s_scene
{
	t_ambient_data	ambient_data;
	t_camera_data	camera_data;
	t_light_data	*light_data;
	t_element_count	element_counts;
	t_sphere_data	*sphere_data;
	t_plane_data	*plane_data;
	t_cylinder_data	*cylinder_data;
	t_cube_data		*cube_data;
	void			*tex_img;
	char			*tex_addr;
	int				tex_width;
	int				tex_height;
	int				tex_bpp;
	int				tex_line;
	int				tex_endian;
	int				tex_loaded;
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

typedef struct s_ray
{
	t_vec3 origin;
	t_vec3 direction;
}	t_ray;

typedef struct s_img
{
    void	*img_ptr;
    char	*addr;
    int		bits_per_pixel;
    int		line_length;
    int		endian;
}	t_img;

typedef struct s_input
{
	int		key_w;
	int		key_a;
	int		key_s;
	int		key_d;
	int		key_q;
	int		key_e;
	int		mode;
	int		render_step;
	int		right_mouse_held;
	int		last_mouse_x;
	int		last_mouse_y;
	double	yaw; 
	double	pitch;
	double	last_time;
	int		key_shift;
	int		selected_type;
	int		edit_mode;
	int		dragging_axis;
	t_vec3	drag_origin;
	t_vec3	drag_axis_dir;
	int		drag_last_x;
	int		drag_last_y;
	double	drag_start_t;
	int		key_ctrl;
	int		left_mouse_held;
	t_vec3	drag_start_dir;
	int		text_input_mode;
	char	text_buffer[TEXT_BUFFER_SIZE];
	int		text_len;
	int		selected_id;
	int		next_sphere_id;
	int		next_plane_id;
	int		next_cylinder_id;
	int		next_light_id;
	int		next_cube_id;
	int		active_property;
	int		text_target;
}	t_input;

typedef struct s_undo_entry
{
	int				action;
	int				obj_type;
	int				obj_id;
	t_vec3			old_value;
	t_sphere_data	del_sphere;
	t_plane_data	del_plane;
	t_cylinder_data	del_cylinder;
	t_light_data	del_light;
	t_cube_data		del_cube;
	int				field;
	double			old_scalar;
}	t_undo_entry;

typedef struct s_scene_backup
{
	t_sphere_data	*sphere_data;
	t_plane_data	*plane_data;
	t_cylinder_data	*cylinder_data;
	t_light_data	*light_data;
	t_cube_data		*cube_data;
	int				sphere_count;
	int				plane_count;
	int				cylinder_count;
	int				light_count;
	int				cube_count;
}	t_scene_backup;

typedef struct s_rt
{
	t_data			*old_data;
	t_img			img;
	t_input			input;
	t_scene_backup	backup;
	t_undo_entry	undo_stack[UNDO_CAPACITY];
	int				undo_count;
}	t_rt;

typedef struct s_hit
{
	double			t;
	t_vec3			point;
	t_vec3			normal;
	unsigned int	red;
	unsigned int	green;
	unsigned int	blue;
	int				is_marker;
	int				obj_type;
	int				obj_index;
	int				checker;
	double			shininess;
	double			specular_strength;
	int				has_texture;
	int				side_hit;
	double			local_h;
}	t_hit;


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
void	validate_cube_data(char **scene);
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
void	create_cube_data(t_scene *scene, char **scene_map);
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
t_vec3	vec3_create(double x, double y, double z);
t_vec3	vec3_add(t_vec3 a, t_vec3 b);
t_vec3	vec3_sub(t_vec3 a, t_vec3 b);
t_vec3	vec3_mul(t_vec3 v, double t);
double	vec3_dot(t_vec3 a, t_vec3 b);
double	vec3_length(t_vec3 v);
t_vec3	vec3_normalize(t_vec3 v);
t_vec3	vec3_cross(t_vec3 a, t_vec3 b);
int		check_seperator_count(char *str, size_t count);
void	render_scene(t_rt *rt_this, int step);
void	my_mlx_pixel_put(t_img *img, int x, int y, int color);
int		intersect_sphere(t_ray ray, t_sphere_data *sphere, t_hit *hit);
int		compute_color(t_scene *scene, t_hit *hit, t_vec3 view_dir,
			int high_quality);
int		is_in_shadow(t_scene *scene, t_vec3 point, t_vec3 normal, t_vec3 light_pos);
int		intersect_plane(t_ray ray, t_plane_data *plane, t_hit *hit);
int		intersect_cylinder(t_ray ray, t_cylinder_data *cy, t_hit *hit);
int		intersect_cube(t_ray ray, t_cube_data *cube, t_hit *hit);
void	init_texture(t_rt *rt);

int		key_press(int keycode, t_rt *rt);
int		key_release(int keycode, t_rt *rt);
void	handle_exit(t_rt *rt_this);
int		render_loop(t_rt *rt_this);
void	init_camera_state(t_rt *rt);
void	update_mouse_look(t_rt *rt);
int		mouse_press(int button, int x, int y, t_rt *rt);
int		mouse_release(int button, int x, int y, t_rt *rt);
double	get_time_seconds(void);
void	update_movement(t_rt *rt);
int		pick_object(t_scene *scene, int x, int y, int *type, int *index);
t_ray	generate_ray(t_scene *scene, int x, int y);
t_ray	generate_ray_at(t_scene *scene, double fx, double fy);
int		project_point(t_scene *scene, t_vec3 point, int *out_x, int *out_y);
int		color_to_int(unsigned int r, unsigned int g, unsigned int b);
t_vec3	get_object_center(t_scene *scene, int type, int index);
void	draw_gizmo(t_rt *rt_this);
void	draw_line_screen(t_rt *rt_this, int x0, int y0, int x1, int y1, int color);
void	set_object_center(t_scene *scene, int type, int index, t_vec3 pos);
int		pick_axis(t_rt *rt, int mx, int my);
void	update_drag(t_rt *rt);
t_vec3	rotate_vector(t_vec3 v, t_vec3 axis, double angle);
t_vec3	get_object_direction(t_scene *scene, int type, int index);
void	set_object_direction(t_scene *scene, int type, int index, t_vec3 dir);
void	update_drag_move(t_rt *rt);
void	update_drag_rotate(t_rt *rt);
double	compute_axis_t(t_ray ray, t_vec3 origin, t_vec3 axis);
int		pick_ring_axis(t_rt *rt, int mx, int my);

void	init_scene_backup(t_rt *rt);
void	free_scene_backup(t_rt *rt);
void	reset_position(t_rt *rt);
void	reset_rotation(t_rt *rt);
void	push_undo(t_rt *rt, int type, int index, int is_rotation,
			t_vec3 old_value);
void	undo_last(t_rt *rt);
t_vec3	get_axis_vector(int axis);
void	present_frame(t_rt *rt);
void	draw_text_input(t_rt *rt);
void	handle_text_input_key(t_rt *rt, int keycode);

void	assign_object_ids(t_rt *rt);
int		find_index_by_id(t_scene *scene, int type, int id);
void	spawn_sphere(t_rt *rt);
void	spawn_plane(t_rt *rt);
void	spawn_cylinder(t_rt *rt);
void	spawn_light(t_rt *rt);
void	spawn_cube(t_rt *rt);
void	cycle_selected_light(t_rt *rt);
void	delete_selected(t_rt *rt);
void	shift_undo_stack(t_rt *rt);
void	shift_remove(t_scene *scene, int type, int index);

void	draw_property_panel(t_rt *rt);
void	cycle_property_field(t_rt *rt);
double	get_property_value(t_scene *scene, int type, int index, int field);
void	set_property_value(t_scene *scene, int type, int index, int field,
			double value);
void	push_property_undo(t_rt *rt, int type, int id, int field,
			double old_scalar);
void	confirm_text_input(t_rt *rt);

#endif
