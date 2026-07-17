SRCS	:= miniRT.c \
			validity_tools/file_verifier.c validity_tools/scene_verifier_pt_one.c validity_tools/scene_verifier_pt_two.c \
			validity_tools/scene_verifier_pt_three.c validity_tools/scene_verifier_pt_four.c validity_tools/scene_verifier_pt_five.c \
			validity_tools/scene_verifier_pt_six.c \
			parser/parse_scene_pt_one.c parser/parse_scene_pt_two.c parser/parse_scene_pt_three.c parser/parse_scene_pt_four.c \
			engine/run_engine.c engine/engine.c engine/vec_utils_pt_one.c engine/vec_utils_pt_two.c \
			engine/sphere_intersections.c engine/lightning.c engine/plane_intersections.c engine/cylinder_intersections.c \
			engine/input.c engine/render_loop.c engine/mouse_mov.c engine/gizmo.c engine/backup.c engine/undo.c \
			engine/text_input.c engine/ids.c engine/spawn.c \
			utils/file_reader.c utils/utils_pt_one.c utils/utils_pt_two.c utils/utils_pt_three.c utils/utils_pt_four.c \
			utils/utils_pt_five.c
LIBS	:= -lm
NAME	:=	miniRT
CC		:=	cc
CFLAGS	:= -Wno-incompatible-pointer-types -O2 -flto -march=native
DEBUG_FLAGS := -Wno-incompatible-pointer-types -Wall -Wextra -Werror -O0 -g -march=x86-64-v2
RM		:=	rm -rf
OBJ_DIR :=	objects
MLX_DIR :=	./minilibx-linux
MLX		:=	$(MLX_DIR)/libmlx.a
OBJ		:=	$(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
GREEN	:=	\033[0;32m
BLUE	:=	\033[0;34m
CYAN	:=	\033[0;36m
RESET	:=	\033[0m
BOLD	:=	\033[1m

all: $(MLX) $(NAME)

$(MLX):
	@make -C $(MLX_DIR)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@printf "$(CYAN)Compiling:$(RESET) %-20s → $(GREEN)%s$(RESET)\n" "$<" "$@"
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@printf "$(BOLD)Linking...$(RESET)\n"
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBS) $(MLX) -lX11 -lXext
	@printf "$(GREEN)Build complete: $(NAME)$(RESET)\n"

clean:
	@$(RM) $(OBJ_DIR)
	@printf "$(BLUE)Cleaned object files$(RESET)\n"

fclean: clean
	@$(RM) $(NAME)
	@make clean -C $(MLX_DIR)
	@printf "$(BLUE)Removed binary $(NAME)$(RESET)\n"

re: fclean all

debug: CFLAGS := $(DEBUG_FLAGS)

debug: fclean $(MLX) $(NAME)

.PHONY: all clean fclean re debug