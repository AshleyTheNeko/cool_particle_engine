##
## ASHLEY'S PROJECT, 2022
## particle
## File description:
## Makefile
##

SHELL	=	/bin/sh
CC	=	gcc
RM	= 	rm -rf

MAIN	=	src/main.c

SRC	=	src/get_socket_line.c	\
		src/manage_clients.c	\
		src/attack_enemy.c	\
		src/init_server.c	\
		src/players.c		\
		src/attacks.c		\
		src/enemies.c

OBJ	=	$(SRC:.c=.o)

LDFLAGS	=	-lm
CFLAGS	=	-Wall -Wextra
CPPFLAGS=	-I include

OK	=	\e[32m
RED	=	\e[91m
END	=	\e[0m

NAME	=	server

DEBUG	=	0
MON	=	0

PHONY	:=	all

define center_text
	echo -e "$(OK)>>>>\t$1$(END)"
endef

ifeq ($(DEBUG), 1)
	CFLAGS += -g -fsanitize=address
	LDFLAGS += -fsanitize=address
endif

ifeq ($(MON), 1)
	CFLAGS += -g -pg
	LDFLAGS += -pg
endif

all: $(NAME)

$(NAME): OBJ += $(MAIN:.c=.o)
$(NAME): $(SRC) $(MAIN)
	@$(call center_text,$(NAME))
	@$(MAKE) -s $(OBJ)
	@$(CC) -o $(NAME) $(OBJ) $(LDFLAGS) && \
	echo -e "$(OK)[DONE]\t$(NAME)$(END)" || \
	echo -e "$(RED)[FAILURE]\t$(NAME)$(END)"

%.o: %.c
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $< && \
	echo -e "$(OK)[OK]\t$<$(END)" || \
	echo -e "$(RED)[FAILURE]\t$<$(END)"

PHONY += rm_prompt
rm_prompt:
	@$(call center_text,Clean)

PHONY += clean
clean: rm_prompt
	@$(RM) $(OBJ) $(MAIN:.c=.o)
	@echo -e "$(RED)$(NAME) : Removed object files.$(END)"

PHONY += fclean
fclean: clean rm_prompt
	@$(RM) $(NAME)
	@echo -e "$(RED)$(NAME) : Removed $(NAME).$(END)"

PHONY += re
re: fclean all

.PHONY: $(PHONY)
