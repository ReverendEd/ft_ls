/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ed <ed@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 20:19:36 by ed                #+#    #+#             */
/*   Updated: 2019/09/03 19:06:45 by ed               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <sys/types.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/xattr.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include "ft_ls.h"

void ft_ls(inputs *input, char *path)
{
	filelist *files = build_file_list();
	nested_dir *dir_list = build_dir_list();
	struct dirent *dp;
	DIR *dir = opendir(path); // Open the directory - dir contains a pointer to manage the dir
	if (dir == NULL)
		ft_putstr("this failed");
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

	final_format = sort_alpha(final_format);
	if (input->filelist->filename)
		final_format = only_selected_files(final_format, input);
	else if (!input->flag_a)
		final_format = remove_dots(final_format);
	if (input->flag_t)
		final_format = sort_by_time(final_format, path);
	if (input->flag_r)
		final_format = reverse_files(final_format);
	if (input->flag_l)
		print_flag_l(final_format, path);
	else
	{
		while (final_format->next)
		{
			ft_putstr(final_format->filename);
			ft_putstr("\n");
			final_format = final_format->next;
		}
	}
}

void print_flag_l(filelist *files, char *path)
{
	struct stat info;
	char *full_path;
	char *final_line;
	int length = 0;
	lstat(full_path, &info);
	full_path = ft_strjoin(path, "/");
	full_path = ft_strjoin(full_path, files->filename);
	length += ft_strlen((getpwuid(info.st_uid))->pw_name);
	length += ft_strlen((getgrgid(info.st_gid))->gr_name);

	while (files->next)
	{
		full_path = ft_strjoin(path, "/");
		full_path = ft_strjoin(full_path, files->filename);
		lstat(full_path, &info);
		final_line = ft_strjoin("", print_type(info));
		final_line = ft_strjoin(final_line, ((info.st_mode & S_IRUSR) ? "r" : "-"));
		final_line = ft_strjoin(final_line, ((info.st_mode & S_IWUSR) ? "w" : "-"));
		final_line = ft_strjoin(final_line, ((info.st_mode & S_IXUSR) ? "x" : "-"));
		final_line = ft_strjoin(final_line, ((info.st_mode & S_IRGRP) ? "r" : "-"));
		final_line = ft_strjoin(final_line, ((info.st_mode & S_IWGRP) ? "w" : "-"));
		final_line = ft_strjoin(final_line, ((info.st_mode & S_IXGRP) ? "x" : "-"));
		final_line = ft_strjoin(final_line, ((info.st_mode & S_IROTH) ? "r" : "-"));
		final_line = ft_strjoin(final_line, ((info.st_mode & S_IWOTH) ? "w" : "-"));
		final_line = ft_strjoin(final_line, ((info.st_mode & S_IXOTH) ? "x" : "-"));
		final_line = normalize_string(final_line, 12);
		final_line = ft_strjoin(final_line, ft_itoa(info.st_nlink));
		final_line = normalize_string(final_line, 16);
		final_line = ft_strjoin(final_line, (getpwuid(info.st_uid))->pw_name);
		final_line = normalize_string(final_line, 11 + length);
		final_line = ft_strjoin(final_line, (getgrgid(info.st_gid)->gr_name));
		final_line = normalize_string(final_line, 18 + length);
		final_line = ft_strjoin(final_line, ft_itoa(info.st_size));
		final_line = normalize_string(final_line, 24 + length);
		final_line = ft_strjoin(final_line, process_date(ctime(&info.st_mtime)));
		final_line = normalize_string(final_line, 39 + length);
		final_line = ft_strjoin(final_line, files->filename);
		final_line = ft_strjoin(final_line, "\n");
		ft_putstr(final_line);
		files = files->next;
	}
}

char *process_date(char *date)
{
	int i = 0;
	int j = ft_strlen(date) - 1;
	char *result;

	while (date[i] != ' ')
		i++;
	while (date[j] != ':')
		j--;
	result = ft_strsub(date, i, j - i);
	return result;
}

char *normalize_string(char *string, int amount)
{
	int i = 0;

	while (i < amount && string[i])
		i++;
	while (string[i] && string[i] != ' ')
		i++;
	while (i < amount)
	{
		string = ft_strjoin(string, " ");
		i++;
	}
	return string;
}

char *print_type(struct stat info)
{
	if (S_ISBLK(info.st_mode))
		return ("b");
	else if (S_ISCHR(info.st_mode))
		return ("c");
	else if (S_ISDIR(info.st_mode))
		return ("d");
	else if (S_ISLNK(info.st_mode))
		return ("l");
	else if (S_ISSOCK(info.st_mode))
		return ("s");
	else if (S_ISFIFO(info.st_mode))
		return ("p");
	else
		return ("-");
}

filelist *sort_by_time(filelist *files, char *path)
{

	filelist *result = build_file_list();
	filelist *start = files;
	struct stat info;
	char *full_path;
	filelist *biggest_time = start;

	while (files->next)
	{
		full_path = ft_strjoin(path, "/");
		full_path = ft_strjoin(full_path, files->filename);
		lstat(full_path, &info);
		files->time = info.st_mtime;
		files = files->next;
	}
	files = start;
	while (check_complete_sort(files) == false)
	{
		while (files->next)
		{
			if (files->time > biggest_time->time)
				biggest_time = files;
			files = files->next;
		}
		add_new_file(result, biggest_time->filename);
		biggest_time->time = 0;
		files = start;
	}
	return result;
}

bool check_complete_sort(filelist *files)
{
	while (files->next)
	{
		if (files->time > 0)
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