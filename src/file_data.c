#include "file_data.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "app.h"

typedef struct
{
	char* module_name;
} module_name_t;

typedef struct
{
	dynamic_list_t* module_name_list; // of type module_name_t
	dynamic_list_t* file_data_list;   // of type file_data_t
} module_name_file_data_lookup_table_t;

static module_name_file_data_lookup_table_t s_module_name_file_data_lookup_table;
static bool s_module_name_file_data_lookup_table_initialized = false;

file_data_t* file_data_lookup_from_module_name(const char* module_name)
{
	for (int i = 0; i < s_module_name_file_data_lookup_table.module_name_list->count; i++)
	{
		module_name_t* module_name_from_table = s_module_name_file_data_lookup_table.module_name_list->elements[i];

		if (strcmp(module_name, module_name_from_table->module_name) == 0)
		{
			file_data_t* module_file_data = s_module_name_file_data_lookup_table.file_data_list->elements[i];
			if (module_file_data->file_type == e_file_type_module_interface)
				return s_module_name_file_data_lookup_table.file_data_list->elements[i];
		}
	}
	
	return NULL;
}

void module_name_file_data_lookup_table_add(const char* module_name, file_data_t* file)
{
	module_name_t* module_name_lookup_table_el = malloc(sizeof(module_name_t));
	module_name_lookup_table_el->module_name = module_name;

	dynamic_list_add(s_module_name_file_data_lookup_table.module_name_list, module_name_lookup_table_el);
	dynamic_list_add(s_module_name_file_data_lookup_table.file_data_list, file);
}

const char* get_filename_from_path(const char* path)
{
	// strrchr - returns pointer to last character in str
	
	const char* last_slash_ptr = strrchr(path, '/');
	//const char* filename = last_slash_ptr + sizeof(char);
	char* filename = malloc(strlen(last_slash_ptr) + 1);	
	strcpy(filename, last_slash_ptr + 1);
	filename[strlen(filename)] = '\0';

	return filename;
}

// Parses a file for its module name and dependencies
void parse_file(file_data_t* f)
{
	typedef enum {
		e_module_interface_name = 0,
		e_module_implementation_name = 1,
		e_module_export_import = 2,
		e_module_import = 3
	} keyword_enum_t;

	const char* keywords[] = {
		"export module ",
		"module ",
		"export import ",
		"import ",
	};

	int keyword_count = (int)sizeof(keywords) / (int)sizeof(keywords[0]);

	FILE* file = fopen(f->path, "r");
	
	if (!file)
		printf("err");

	char line[128];
	
	f->module_name = "";

	while (fgets(line, 128, file))
	{
		if (strstr(line, "module;"))
			continue;
		
		for (int i = 0; i < keyword_count; i++)
		{
			char* found = strstr(line, keywords[i]);

			if (!found || found != line)
				continue;
	
			char* line_cp = malloc(strlen(line) + 1);
			strcpy(line_cp, line);
			
			int keyword_len = strlen(keywords[i]);
			line_cp = line_cp + keyword_len;

			char* semicolon_ptr = strchr(line_cp, ';');
			int semicolon_pos = (int)(semicolon_ptr - line_cp);

			for (int j = semicolon_pos; j < strlen(line_cp); j++)
			{
				line_cp[j] = 0;
			}

			switch (i)
			{
				case e_module_interface_name:
				case e_module_implementation_name:
				{
					// set module_name
					
					//f->module_name = line_cp; 
					f->module_name = malloc(strlen(line_cp) + 1);
					strcpy(f->module_name, line_cp);
					f->module_name[strlen(line_cp)] = '\0';

					module_name_file_data_lookup_table_add(f->module_name, f);

					break;
				}
				case e_module_import:
				case e_module_export_import:
				{
					// add to dependency list
					
					file_dependency_data_t* file_dep_data = malloc(sizeof(file_dependency_data_t));
					file_dep_data->module_name = line_cp;
		
					dynamic_list_add(f->dependency_list, file_dep_data);
					break;
				}
				default: continue;
			}

			break;
		}
	}
}

file_data_t* file_data_init(const char* path)
{
	if (!s_module_name_file_data_lookup_table_initialized)
	{
		s_module_name_file_data_lookup_table.module_name_list = dynamic_list_init(50, sizeof(module_name_t));
		s_module_name_file_data_lookup_table.file_data_list = dynamic_list_init(50, sizeof(file_data_t));
		s_module_name_file_data_lookup_table_initialized = true;
	}

	file_data_t* f = malloc(sizeof(file_data_t));
	
	f->path = path;
	f->dependency_list = dynamic_list_init(5, sizeof(file_dependency_data_t));
	
	if (strcmp(f->path, FILE_DATA_ROOT) != 0)
	{
		f->filename = get_filename_from_path(f->path); 

		// sets module_name and dependency_list
		parse_file(f);
	}


	return f;
}

void file_data_free(file_data_t* f)
{
	for (int i = 0; i < f->dependency_list->count; i++)
	{
		file_dependency_data_t* dep = f->dependency_list->elements[i];
		free(dep->module_name);
	}

	dynamic_list_free(f->dependency_list);
	free(f->path);
	free(f->filename);
	free(f->module_name);
	free(f);
}

void file_data_print(file_data_t* f)
{
	if (strcmp(f->path, FILE_DATA_ROOT) == 0)
		return;

	printf("%s\n", f->path);
}

