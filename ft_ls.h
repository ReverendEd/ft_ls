/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ed <ed@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 20:19:35 by ed                #+#    #+#             */
/*   Updated: 2019/08/25 15:28:51 by ed               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# define REQUIRE(x, y) if (!x) return (y);

# include "./libft/libft.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

typedef int bool;
#define true 1
#define false 0

typedef struct s_files
{
	char *filename;
	struct s_files *next;
}				filelist;

typedef struct s_inputs
{
	bool		flag_l;
	bool		flag_R;
	bool		flag_a;
	bool		flag_r;
	bool		flag_t;
	bool		flag_error;
	struct s_files *filelist;
}				inputs;

char *ERROR_MESSAGES[] = {
	"all is well",
	"¡parse_input: no arguments",
	"¡read_flag: invalid flag",
	
};

inputs *build_input_struct();
int parse_input(int argc, char **argv, inputs *session_inputs);
int read_flag(char *input, inputs *session_inputs);
void print_inputs(inputs *input);
void read_file(char *input, inputs *session_inputs);

#endif
