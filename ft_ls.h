/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ed <ed@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 20:19:35 by ed                #+#    #+#             */
/*   Updated: 2019/08/30 12:50:32 by ed               ###   ########.fr       */
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
	int time;
	struct s_files *next;
	struct s_files *previous;
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

typedef struct nested_directories
{
	char *dirname;
	struct nested_directories *next;
	struct nested_directories *start;
}				nested_dir;

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
void add_new_dir(nested_dir *dir_list, char *dir_name);
nested_dir *build_dir_list();
void ft_ls(inputs *input, char *path);
char *build_nested_dirname(char *dirname, char *current_path);
filelist *build_file_list();
void add_new_file(filelist *file_list, char *file_name);
filelist *reverse_files(filelist *files);
filelist *remove_dots(filelist *files);
filelist *only_selected_files(filelist *files, inputs *input);
void print_file_list(filelist *files, inputs *input, char *path);
filelist *sort_by_time(filelist *files, char *path);
bool check_complete_sort(filelist *files);

#endif
