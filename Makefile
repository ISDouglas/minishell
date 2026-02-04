# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes -g -O0

# OS detection
UNAME := $(shell uname)

# Flags for MacOS otherwise Linux
ifeq ($(UNAME), Darwin)
    LDFLAGS = -L/opt/homebrew/opt/readline/lib -I/opt/homebrew/opt/readline/include -lreadline
else
    LDFLAGS = -lreadline
endif

# Directories
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = includes
LIBFT_DIR = Libft
LIBFT_OBJ_DIR = $(LIBFT_DIR)/obj

# Files
NAME = minishell
SRC_FILES = \
	$(SRC_DIR)/minishell.c \
	$(SRC_DIR)/lexer/lexer.c \
	$(SRC_DIR)/lexer/lexer_utils.c \
	$(SRC_DIR)/lexer/lexer_exe_unit.c \
	$(SRC_DIR)/lexer/lexer_dquote.c \
	$(SRC_DIR)/lexer/lexer_cmd_block.c \
	$(SRC_DIR)/execution/execute.c \
	$(SRC_DIR)/execution/execute_fill_cmd.c \
	$(SRC_DIR)/execution/execute_fill_utiles.c \
	$(SRC_DIR)/execution/execute_cmd.c \
	$(SRC_DIR)/execution/execute_pipe.c \
	$(SRC_DIR)/execution/execute_utils.c \
	$(SRC_DIR)/execution/execute_built_in.c \
	$(SRC_DIR)/buildin/environment.c \
	$(SRC_DIR)/buildin/cd.c \
	$(SRC_DIR)/buildin/echo.c \
	$(SRC_DIR)/buildin/unset.c \
	$(SRC_DIR)/buildin/export.c \
	$(SRC_DIR)/utils/utils.c \
	$(SRC_DIR)/utils/ft_error_ctr.c \
	$(SRC_DIR)/utils/free_utils.c \
	$(SRC_DIR)/utils/ft_log_use.c \
	$(SRC_DIR)/utils/ft_file_path_ctr.c
 
# Bonus Files
NAME_B = minishell_bonus
SRC_FILES_B = \
	$(SRC_DIR)/minishell_bonus.c \
	$(SRC_DIR)/lexer/lexer_bonus.c \
	$(SRC_DIR)/lexer/lexer_utils_bonus.c \
	$(SRC_DIR)/lexer/lexer_exe_unit_bonus.c \
	$(SRC_DIR)/lexer/lexer_dquote.c \
	$(SRC_DIR)/lexer/lexer_cmd_block_bonus.c \
	$(SRC_DIR)/lexer/lexer_wildcard.c \
	$(SRC_DIR)/lexer/lexer_reread.c \
	$(SRC_DIR)/execution/execute_bonus.c \
	$(SRC_DIR)/execution/execute_jump.c \
	$(SRC_DIR)/execution/execute_fill_cmd.c \
	$(SRC_DIR)/execution/execute_fill_utiles.c \
	$(SRC_DIR)/execution/execute_cmd_bonus.c \
	$(SRC_DIR)/execution/execute_pipe_bonus.c \
	$(SRC_DIR)/execution/execute_utils.c \
	$(SRC_DIR)/execution/execute_built_in_bonus.c \
	$(SRC_DIR)/buildin/environment.c \
	$(SRC_DIR)/buildin/cd_bonus.c \
	$(SRC_DIR)/buildin/echo.c \
	$(SRC_DIR)/buildin/unset.c \
	$(SRC_DIR)/buildin/export.c \
	$(SRC_DIR)/utils/utils.c \
	$(SRC_DIR)/utils/utils_bonus.c \
	$(SRC_DIR)/utils/ft_error_ctr_bonus.c \
	$(SRC_DIR)/utils/free_bonus.c \
	$(SRC_DIR)/utils/free_utils_bonus.c \
	$(SRC_DIR)/utils/ft_syntax_err.c \
	$(SRC_DIR)/utils/ft_log_use.c \
	$(SRC_DIR)/utils/ft_file_path_ctr.c

# Libft files
LIBFT_SRC = \
	$(LIBFT_DIR)/ft_split.c \
	$(LIBFT_DIR)/ft_strncmp.c \
	$(LIBFT_DIR)/ft_memcpy.c \
	$(LIBFT_DIR)/ft_strlen.c \
	$(LIBFT_DIR)/ft_strdup.c \
	$(LIBFT_DIR)/ft_strchr.c \
	$(LIBFT_DIR)/ft_strlcpy.c \
	$(LIBFT_DIR)/ft_toupper.c \
	$(LIBFT_DIR)/ft_memset.c \
	$(LIBFT_DIR)/ft_bzero.c \
	$(LIBFT_DIR)/ft_strjoin.c \
	$(LIBFT_DIR)/ft_putchar_fd.c \
	$(LIBFT_DIR)/ft_putstr_fd.c \
	$(LIBFT_DIR)/ft_putnbr_fd.c \
	$(LIBFT_DIR)/get_next_line_lan.c \
	$(LIBFT_DIR)/ft_atoi.c \
	$(LIBFT_DIR)/ft_itoa.c \
	$(LIBFT_DIR)/ft_isalnum.c \
	$(LIBFT_DIR)/ft_isalpha.c \
	$(LIBFT_DIR)/ft_isdigit.c


LIBFT_OBJ = $(LIBFT_SRC:$(LIBFT_DIR)/%.c=$(LIBFT_OBJ_DIR)/%.o)
OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC_FILES:.c=.o)))
OBJ_FILES_B = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC_FILES_B:.c=.o)))

# Targets
all: $(NAME)
bonus: $(NAME_B)

$(NAME): $(OBJ_FILES) $(LIBFT_OBJ)
	@echo "Linking object files to create the executable $(NAME)..."
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ_FILES) $(LIBFT_OBJ) $(LDFLAGS)
	@echo "Executable $(NAME) created successfully!"

$(NAME_B): $(OBJ_FILES_B) $(LIBFT_OBJ)
	@echo "Linking object files to create the executable $(NAME_B)..."
	$(CC) $(CFLAGS) -o $(NAME_B) $(OBJ_FILES_B) $(LIBFT_OBJ) $(LDFLAGS)
	@echo "Executable $(NAME_B) created successfully!"

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/*/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(LIBFT_OBJ_DIR)/%.o: $(LIBFT_DIR)/%.c
	@mkdir -p $(LIBFT_OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(LIBFT_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(LIBFT_OBJ_DIR)

fclean: clean
	rm -f $(NAME) $(NAME_B)

re: fclean all

.PHONY: all clean fclean re bonus

#export PS1="\e[32m🌟 Let's test BASH$ \e[0m"
#export PS1="\e[32m❤️ Let's test Minishell on bash$ \e[0m"
#without --trace-children=yes
#valgrind  --suppressions=valgrind.supp --leak-check=full --show-leak-kinds=all  --track-origins=yes ./minishell
#valgrind  --suppressions=valgrind.supp --leak-check=full --show-leak-kinds=all  --track-origins=yes ./minishell_bonus