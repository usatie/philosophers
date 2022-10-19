# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: susami <susami@student.42tokyo.jp>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/19 18:48:58 by susami            #+#    #+#              #
#    Updated: 2022/10/19 11:14:22 by susami           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#############
# Variables #
#############

NAME		=	philo

CC			=	cc
CFLAGS		=	-Wall -Werror -Wextra

SRC_DIR		=	src

INCLUDE_DIR	=	include
INCLUDES	=	-I $(INCLUDE_DIR)
SRCDIRS		=	$(shell find $(SRC_DIR) -type d)
INCLUDES 	+=	$(addprefix -I,$(SRCDIRS))

SRCS		=	$(SRC_DIR)/init.c							\
				$(SRC_DIR)/log.c							\
				$(SRC_DIR)/main.c							\
				$(SRC_DIR)/monitor.c						\
				$(SRC_DIR)/philosopher.c					\
				$(SRC_DIR)/simulation.c						\
				$(SRC_DIR)/time.c							\


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

debug: CFLAGS += -fsanitize=thread -Weverything -Wno-padded -Wno-strict-prototypes -Wno-packed
debug: re

norm:
	norminette $(SRC_DIR) $(INCLUDE_DIR)
	@echo "norminette OK :D"

test: debug norm
	./test.sh

.PHONY: all clean fclean re bonus debug norm test
-include $(DEPS)
