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
	$(SRC_DIR)/lexer.c \
	$(SRC_DIR)/lexer_utils.c \
	$(SRC_DIR)/lexer_exe_unit.c \
	$(SRC_DIR)/utils.c \
	$(SRC_DIR)/environment.c \
	$(SRC_DIR)/execute.c \
	$(SRC_DIR)/execute_fill_cmd.c \
	$(SRC_DIR)/execute_cmd.c \
	$(SRC_DIR)/execute_pipe.c \
	$(SRC_DIR)/execute_utils.c \
	$(SRC_DIR)/execute_built_in.c \
	$(SRC_DIR)/cd.c \
	$(SRC_DIR)/echo.c \
	$(SRC_DIR)/unset.c \
	$(SRC_DIR)/export.c \
	$(SRC_DIR)/ft_error_ctr.c \
	$(SRC_DIR)/ft_test_use.c
	 
	
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
	$(LIBFT_DIR)/ft_putnbr_fd.c \
	$(LIBFT_DIR)/get_next_line_lan.c


LIBFT_OBJ = $(LIBFT_SRC:$(LIBFT_DIR)/%.c=$(LIBFT_OBJ_DIR)/%.o)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Targets
all: $(NAME)

$(NAME): $(OBJ_FILES) $(LIBFT_OBJ)
	@echo "Linking object files to create the executable $(NAME)..."
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ_FILES) $(LIBFT_OBJ) $(LDFLAGS)
	@echo "Executable $(NAME) created successfully!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(LIBFT_OBJ_DIR)/%.o: $(LIBFT_DIR)/%.c
	@mkdir -p $(LIBFT_OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(LIBFT_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(LIBFT_OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

#export PS1="\e[32m🌟 Let's test BASH$ \e[0m"
#export PS1="\e[32m❤️ Let's test Minishell on bash$ \e[0m"
#without --trace-children=yes
#valgrind  --suppressions=valgrind.supp --leak-check=full --show-leak-kinds=all  --track-origins=yes ./minishell