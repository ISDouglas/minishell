# Nom de la bibliothèque
NAME = libft.a

# Compilateur et options de compilation
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -O0

# Commandes
AR = ar rcs
RM = rm -f

# Fichiers sources
SRCS = \
	ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
	ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c \
    ft_strlcat.c ft_tolower.c ft_toupper.c ft_strchr.c ft_strrchr.c \
	ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c ft_calloc.c \
	ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c \
	ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c \
	ft_putnbr_fd.c ft_atoi_base.c get_next_line.c

# Fichiers objets
OBJS = $(SRCS:.c=.o)

# Fichiers bonus
BONUS_SRCS = \
	ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c \
	ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstdelone_bonus.c \
	ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c

# Fichiers objets bonus
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

# Fichier header
HEADERS = libft.h

# Cible par défaut
all: $(NAME)

# Création de la bibliothèque
$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)
	@echo "Création de $(NAME) terminée."

# Création de la bibliothèque averc bonus
bonus: $(OBJS) $(BONUS_OBJS)
	$(AR) $(NAME) $(OBJS) $(BONUS_OBJS)
	@echo "Bonus ajoutés à $(NAME)."

# Compilation des fichiers .c en .o
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -g -o $@

# Nettoyage des fichiers objets
clean:
	$(RM) $(OBJS) $(BONUS_OBJS)
	@echo "Fichiers objets supprimés."

# Nettoyage complet
fclean: clean
	$(RM) $(NAME)
	@echo "$(NAME) supprimée."

# Recompilation complète
re: fclean all

# Commandes
.PHONY: all clean fclean re
