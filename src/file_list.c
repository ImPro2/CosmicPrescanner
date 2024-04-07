#include "file_list.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "app.h"

file_list_t* file_list_init(const char* root_path)
{
	file_list_t* fl = malloc(sizeof(file_list_t));

	fl->file_list = dynamic_list_init(50, sizeof(file_data_t));

	return fl;
}

void file_list_free(file_list_t* fl)
{
	dynamic_list_free(fl->file_list);
	free(fl);
}

void parse_dir_recurse(file_list_t* fl, const char* path)
{
	DIR* d = opendir(path);
	if (!d) printf("err, invalid dir");

	struct dirent* dir;

	while ((dir = readdir(d)) != NULL)
	{
		if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
			continue;	

		char file_path[255];
		sprintf(file_path, "%s/%s", path, dir->d_name);

		if (dir->d_type == DT_DIR)
			parse_dir_recurse(fl, file_path);
		else if (dir->d_type == DT_REG)
		{
			//file_data_t* root_f = fl->file_list->elements[0];
			//if (strcmp(file_path, root_f->path) == 0)
			//	continue;
			
			char* data = malloc(sizeof(char) * (strlen(file_path) + 1));
			strcpy(data, file_path);
			data[strlen(file_path)] = '\0';
		
			// first check if extension is .cpp or .cxx:

			file_type_t file_type;

			if (strstr(data, ".cxx"))
				file_type = e_file_type_module_interface;
			else if (strstr(data, ".cpp"))
				file_type = e_file_type_module_implementation;
			else
				continue;
			
			file_data_t* f = file_data_init(data);
			f->file_type = file_type;
			//file_list_add(fl, f);
			dynamic_list_add(fl->file_list, f);
		}
	}
}

void file_list_parse_dir(file_list_t* fl)
{
	app_data_t* app_data = get_app_data();

	file_data_t* root_f = file_data_init(app_data->root_file_path);
	file_list_add(fl, root_f);

	parse_dir_recurse(fl, app_data->root_path);	
}

void file_list_add(file_list_t* fl, file_data_t* f)
{
	dynamic_list_add(fl->file_list, f);
}

void file_list_print(file_list_t* fl)
{
	for (int i = 0; i < fl->file_list->count; i++)
	{
		file_data_t* f = fl->file_list->elements[i];
		file_data_print(f);
	}
}
