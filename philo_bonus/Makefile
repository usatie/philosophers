# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/19 18:48:58 by susami            #+#    #+#              #
#    Updated: 2022/10/24 18:58:39 by susami           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#############
# Variables #
#############

NAME		=	philo_bonus

CC			=	cc
CFLAGS		=	-Wall -Werror -Wextra

ifneq ($(MAX_PHILO),)
CFLAGS		+= -D MAX_PHILO=$(MAX_PHILO)
endif

SRC_DIR		=	src

INCLUDE_DIR	=	include
INCLUDES	=	-I $(INCLUDE_DIR)
SRCDIRS		=	$(shell find $(SRC_DIR) -type d)
INCLUDES 	+=	$(addprefix -I,$(SRCDIRS))

SRCS		=	$(SRC_DIR)/main.c							\
				$(SRC_DIR)/init.c							\
				$(SRC_DIR)/util/time.c						\
				$(SRC_DIR)/util/sleep.c						\
				$(SRC_DIR)/util/err_exit.c					\
				$(SRC_DIR)/argparse/argparse.c				\
				$(SRC_DIR)/simulation/simulation.c			\
				$(SRC_DIR)/simulation/log.c					\
				$(SRC_DIR)/simulation/philosopher.c			\
				$(SRC_DIR)/simulation/philosopher_action.c	\
				$(SRC_DIR)/simulation/monitor.c				\
				$(SRC_DIR)/libftsubset/ft_isdigit.c			\
				$(SRC_DIR)/libftsubset/ft_isspace.c			\
				$(SRC_DIR)/libftsubset/ft_strlen.c			\
				$(SRC_DIR)/libftsubset/ft_strncmp.c			\
				$(SRC_DIR)/libftsubset/ft_strtol.c			\
				$(SRC_DIR)/libftsubset/ft_putstr_fd.c		\
				$(SRC_DIR)/libftsubset/ft_strlcat.c			\
				$(SRC_DIR)/libftsubset/ft_calloc.c			\
				$(SRC_DIR)/libftsubset/ft_itoa.c			\


OBJ_DIR		=	objs
OBJS		=	$(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS		=	$(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.d)
.PHONY: $(DEPS)

##########################
# Platform Compatibility #
##########################

# LINUX | OSX | ARM
UNAME_S := $(shell uname -s)
UNAME_P := $(shell uname -p)

# Linux
ifeq ($(UNAME_S),Linux)
	CFLAGS += -D LINUX
debug: CFLAGS += -Wno-conversion -Wno-double-promotion
debug: CC = clang
endif

# macos x86
ifeq ($(UNAME_S),Darwin)
	CFLAGS += -D OSX
	FRAMEWORK = -framework OpenGL -framework AppKit
debug: CFLAGS += -Wno-reserved-id-macro
endif

# macos ARM (m1/m2...)
ifneq ($(filter arm%, $(UNAME_P)),)
	CFLAGS += -D ARM
	FRAMEWORK = -framework OpenGL -framework AppKit
debug: CFLAGS += -Wno-poison-system-directories
endif

#################
# General rules #
#################

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(NAME)

clean:
	$(RM) -rf $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

debug: CFLAGS += -fsanitize=thread -Weverything -Wno-padded -Wno-strict-prototypes -Wno-packed -Wno-cast-qual
debug: re

norm:
	norminette $(SRC_DIR) $(INCLUDE_DIR)
	@echo "norminette OK :D"

test: debug norm
	./test.sh

.PHONY: all clean fclean re bonus debug norm test
-include $(DEPS)
