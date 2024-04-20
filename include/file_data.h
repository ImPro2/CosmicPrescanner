#ifndef FILE_DATA_H
#define FILE_DATA_H

#include "dynamic_list.h"

typedef enum
{
	e_file_type_module_interface,     // .cxx
	e_file_type_module_implementation // .cpp
} file_type_t;

struct file_data_t;

typedef struct
{
	const char* module_name;
	struct file_data_t* file_data;
} file_dependency_data_t;

typedef struct file_data_t
{
	const char*  path;
	const char*  filename;
	char*  module_name;
	
	file_type_t  file_type;

	dynamic_list_t* dependency_list; // of type file_dependency_data_t
} file_data_t;

#define FILE_DATA_ROOT "root"

file_data_t* file_data_init(const char* path);
void         file_data_free(file_data_t* f);

file_data_t* file_data_lookup_from_module_name(const char* file_path);

void file_data_print(file_data_t* f);

#endif
