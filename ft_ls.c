/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ed <ed@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 20:19:36 by ed                #+#    #+#             */
/*   Updated: 2019/08/30 12:52:39 by ed               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// backtracking to handle the recursive part

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include "ft_ls.h"

void ft_ls(inputs *input, char *path)
{
	printf("\n\n%s\n", path);
	filelist *files = build_file_list();
	nested_dir *dir_list = build_dir_list();
	struct dirent *dp;
	DIR *dir = opendir(path); // Open the directory - dir contains a pointer to manage the dir
	if (dir == NULL)
		printf("this failed");
	while ((dp = readdir(dir))) // if dp is null, there's no more content to read
	{
		if (dp->d_type == 4) // if the filetype is a directory
			add_new_dir(dir_list, dp->d_name);
		add_new_file(files, dp->d_name); // used to be printf=>d_name
	}
	print_file_list(files, input, path);
	while (input->flag_R && dir_list->next && input->filelist->filename == NULL) // handling -R with recursive ft_ls calls
	{
		ft_ls(input, build_nested_dirname(dir_list->dirname, path));
		dir_list = dir_list->next;
	}
	closedir(dir); // close the dir
}

void print_file_list(filelist *files, inputs *input, char *path)
{
	filelist *final_format = files;
	
	if (input->filelist->filename)
		final_format = only_selected_files(final_format, input);
	else if (!input->flag_a)
		final_format = remove_dots(final_format);

	if (input->flag_t)
		final_format = sort_by_time(files, path);
	if (input->flag_r)
		final_format = reverse_files(final_format);
	while (final_format->next)
	{
		printf("%s\n", final_format->filename);
		final_format = final_format->next;
	}
}

filelist *sort_by_time(filelist *files, char *path)
{
	
	filelist *result = build_file_list();
	filelist *start = files;
	struct stat info;
	char *full_path;
	bool sort_completed = false;
	int biggest_time;
	
	while (files->next)
	{
		full_path = ft_strjoin(path, files->filename);
		stat(full_path, &info);
		files->time = info.st_mtimespec.tv_sec;
		files = files->next;
	}
	files = start;

	while (sort_completed == false)
	{
		biggest_time = 0;
		while (files->next)
		{
			if (files->time > biggest_time && files->filename != NULL)
			{
				biggest_time = files->time;
				add_new_file(result, files->filename);
				files->filename = NULL;
			}
			files = files->next;
		}
		if (check_complete_sort(start) == true)
			sort_completed = true;
	}
	return result;
}

bool check_complete_sort(filelist *files)
{
	while (files->next)
	{
		if (files->filename != NULL)
			return false;
		files = files->next;
	}
	return true;
}

filelist *only_selected_files(filelist *files, inputs *input)
{
	filelist *result = build_file_list();
	filelist *search = input->filelist;
	bool search_found = false;

	while (search->next)
	{
		while (files->next)
		{
			if (ft_strcmp(search->filename, files->filename) == 0)
			{
				search_found = true;
				add_new_file(result, files->filename);
			}
			files = files->next;
		}
		while (files->previous)
			files = files->previous;
		if (!search_found)
			printf("No such file or directory\n"); // fill this out later
		search_found = false;
		search = search->next;
	}

	return result;
}

filelist *remove_dots(filelist *files)
{
	filelist *result = build_file_list();

	while (files->next)
	{
		if (files->filename[0] != '.')
			add_new_file(result, files->filename);
		files = files->next;
	}
	// free the old list!
	return result;
}

filelist *reverse_files(filelist *files)
{
	filelist *result = build_file_list();

	while (files->next)
		files = files->next;
	while (files->previous)
	{
		files = files->previous;
		add_new_file(result, files->filename);
	}
	// add_new_file(result, files->filename);
	// free the previous list!
	return result;
}

filelist *build_file_list()
{
	filelist *output = (filelist *)malloc(sizeof(filelist));
	output->filename = NULL;
	output->next = NULL;
	output->previous = NULL;
	return output;
}

void add_new_file(filelist *file_list, char *file_name)
{
	while (file_list->next)
		file_list = file_list->next;
	file_list->filename = file_name;
	file_list->next = (filelist *)malloc(sizeof(filelist));
	file_list->next->filename = NULL;
	file_list->next->next = NULL;
	file_list->next->previous = file_list;
}

char *build_nested_dirname(char *dirname, char *current_path)
{
	current_path = ft_strjoin(current_path, "/");
	dirname = ft_strjoin(current_path, dirname);
	return dirname;
}

nested_dir *build_dir_list()
{
	nested_dir *output = (nested_dir *)(malloc(sizeof(nested_dir)));
	output->next = NULL;
	output->start = output;
	return output;
}

void add_new_dir(nested_dir *dir_list, char *dir_name)
{
	if (ft_strcmp(dir_name, ".") != 0 && ft_strcmp(dir_name, "..") != 0) // do NOT add the current and previous dir to list
	{
		while (dir_list->next)
			dir_list = dir_list->next;
		dir_list->dirname = dir_name;
		dir_list->next = (nested_dir *)malloc(sizeof(nested_dir));
		dir_list->next->next = NULL;
		dir_list->next->start = dir_list->start;
	}
}