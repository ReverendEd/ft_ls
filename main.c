/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ed <ed@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 21:07:51 by ed                #+#    #+#             */
/*   Updated: 2019/09/03 19:24:16 by ed               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include <stdlib.h>
#include "ft_ls.h"
#include "./libft/libft.h"
#include "./ft_ls.c"

int main(int argc, char **argv)
{
    // begin input parsing
    inputs *session_inputs = build_input_struct();
    inputs *mult_dir = build_input_struct();
    filelist *files;
    DIR *dir;
    char *dirname;

    int parse_input_status = parse_input(argc, argv, session_inputs);
    session_inputs->filelist = sort_alpha(session_inputs->filelist);
    // ft_putstr("parse_input: ");
    // ft_putstr(ERROR_MESSAGES[parse_input_status]);
    // ft_putstr("\n");
    // print_inputs(session_inputs);
    if (parse_input_status != 0 || session_inputs->flag_error)
        return 0;
    // end input parsing
    files = session_inputs->filelist;
    session_inputs->filelist = build_file_list();
    while (files->next)
    {
        dirname = ft_strjoin("./", files->filename);
        dir = opendir(files->filename);
        mult_dir->flag_l = session_inputs->flag_l;
        mult_dir->flag_R = session_inputs->flag_R;
        mult_dir->flag_a = session_inputs->flag_a;
        mult_dir->flag_r = session_inputs->flag_r;
        mult_dir->flag_t = session_inputs->flag_t;
        if (dir != NULL)
        {
            ft_putstr(files->filename);
            ft_putstr(":\n\n");
            ft_ls(mult_dir, files->filename);
            ft_putstr("\n");
        }
        else
            add_new_file(session_inputs->filelist, files->filename);
        files = files->next;
    }
        ft_ls(session_inputs, ".");
    return 0;
}

void print_inputs(inputs *input)
{
    // printf("l: %d, R: %d, a: %d, r: %d, t: %d, error: %d\n", input->flag_l, input->flag_R, input->flag_a, input->flag_r, input->flag_t, input->flag_error);
    filelist *files = input->filelist;
    // printf("files:\n");
    while (files->next)
    {
        // printf("%s\n", files->filename);
        files = files->next;
    }
}

filelist *sort_alpha(filelist *files)
{
	filelist *result = build_file_list();
	filelist *start = files;
	filelist *biggest_name = start;

	while (is_alpha_sort_completed(files) == false)
	{
		while (files->next)
		{
			if (ft_strcmp(files->filename, biggest_name->filename) > 0)
				biggest_name = files;
			files = files->next;
		}
		add_new_file(result, biggest_name->filename);
		biggest_name->filename = "";
		files = start;
	}
	result = reverse_files(result);
	return result;
}

bool is_alpha_sort_completed(filelist *files)
{
	while (files->next)
	{
		if (ft_strlen(files->filename) != 0)
			return false;
		files = files->next;
	}
	return true;
}

inputs *build_input_struct()
{
    inputs *input_struct = (inputs *)malloc(sizeof(inputs));
    input_struct->flag_l = false;
    input_struct->flag_R = false;
    input_struct->flag_a = false;
    input_struct->flag_r = false;
    input_struct->flag_t = false;
    input_struct->flag_error = false;
    input_struct->filelist = (filelist *)malloc(sizeof(filelist));
    input_struct->filelist->filename = NULL;
    input_struct->filelist->next = NULL;
    return input_struct;
}

int parse_input(int argc, char **argv, inputs *session_inputs)
{
    REQUIRE(argc, 1);
    int status = 0;
    bool files_started = false;

    int i = 1;
    if (argc == 1)
        return (status);

    while (i < argc)
    {
        if (ft_strlen(argv[i]) > 1 && argv[i][0] == '-' && files_started == false) // is flag
            status = read_flag(argv[i], session_inputs);
        else // is file
        {
            read_file(argv[i], session_inputs);
            files_started = true;
        }
        i++;
    }
    return (status);
}

int read_flag(char *input, inputs *session_inputs)
{
    input++;
    while (*input)
    {
        if (*input == 'l')
            session_inputs->flag_l = true;
        if (*input == 'R')
            session_inputs->flag_R = true;
        if (*input == 'a')
            session_inputs->flag_a = true;
        if (*input == 'r')
            session_inputs->flag_r = true;
        if (*input == 't')
            session_inputs->flag_t = true;
        if (*input != 'l' && *input != 'R' && *input != 'a' && *input != 'r' && *input != 't')
        {
            session_inputs->flag_error = true;
            return (2);
        }
        input++;
    }
    return (0);
}

void read_file(char *input, inputs *session_inputs)
{
    filelist *list = session_inputs->filelist;
    while (list->next)
        list = list->next; // go to the end of the list
    list->filename = (char *)malloc(sizeof(char) * ft_strlen(input) + 1);
    list->filename = input;
    list->next = (filelist *)malloc(sizeof(filelist));
    list->next->filename = NULL;
    list->next->next = NULL;
    // here we need to see if the file is a directory. if it is then we will make ls calls for each directory as well as ./
}