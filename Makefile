SRCS	:= miniRT.c \
			validity_tools/file_verifier.c validity_tools/scene_verifier_pt_one.c validity_tools/scene_verifier_pt_two.c \
			validity_tools/scene_verifier_pt_three.c validity_tools/scene_verifier_pt_four.c validity_tools/scene_verifier_pt_five.c \
			validity_tools/scene_verifier_pt_six.c validity_tools/scene_verifier_pt_seven.c \
			parser/parse_scene_pt_one.c parser/parse_scene_pt_two.c parser/parse_scene_pt_three.c parser/parse_scene_pt_four.c \
			parser/parse_scene_pt_five.c parser/parse_scene_pt_six.c parser/parse_scene_pt_seven.c parser/parse_scene_pt_eight.c \
			parser/parse_scene_pt_nine.c parser/parse_scene_pt_ten.c parser/parse_scene_pt_eleven.c parser/parse_scene_pt_twelve.c \
			parser/parse_scene_pt_thirteen.c \
			engine/engine.c \
			utils/file_reader.c utils/utils_pt_one.c utils/utils_pt_two.c utils/utils_pt_three.c utils/utils_pt_four.c \
			tmp_funcs/print_scene.c
LIBS	:= -lm
NAME	:=	miniRT
CC		:=	cc
CFLAGS	:=	-Wall -Wextra -Werror
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
	@printf "$(GREEN)✔ Build complete: $(NAME)$(RESET)\n"

clean:
	@$(RM) $(OBJ_DIR)
	@printf "$(BLUE)🧹 Cleaned object files$(RESET)\n"

fclean: clean
	@$(RM) $(NAME)
	@make clean -C $(MLX_DIR)
	@printf "$(BLUE)🧹 Removed binary $(NAME)$(RESET)\n"

re: fclean all

.PHONY: all clean fclean re