# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ed <ed@student.42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/08/22 20:19:29 by ed                #+#    #+#              #
#    Updated: 2019/08/22 20:55:33 by ed               ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

LIBFT_NAME=libft.a

LIBFT_INCLUDES=./libft

libft-compile: $(LIBFT_SRCS) ./libft/libft.h
	@gcc -Wall -Wextra -Werror -I$(LIBFT_INCLUDES) -c ./libft/*.c
	@ar rc $(LIBFT_NAME) ./libft/*.o
	@ranlib $(LIBFT_NAME)

libft-clean:
	@/bin/rm -f ./libft/*.o

libft-fclean: clean
	@/bin/rm -f ./libft/libft.a




NAME = ft_ls
SRCS = srcs/*.c

LIBDIR = libft
SRCDIR = srcs

all: $(NAME)

$(NAME): fclean libft-compile
	gcc -Wall -Wextra -Werror -o $(NAME) $(SRCS) -I $(SRCDIR) -I $(LIBDIR) -L $(LIBDIR)  -lft

debug:
	gcc -Wall -Wextra -Werror -fsanitize=address -g srcs/ft_ls.c -o $(NAME) $(SRCS) -I $(SRCDIR) -I $(LIBDIR) -L $(LIBDIR) -lft 
	./ft_ls.out >> debug.txt

clean:
	/bin/rm -rf *.o

fclean: clean
	/bin/rm -rf $(NAME)

re: fclean all