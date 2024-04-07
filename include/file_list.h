#ifndef FILE_LIST_H
#define FILE_LIST_H

#include "dynamic_list.h"
#include "file_data.h"

typedef struct {
	dynamic_list_t* file_list; // of type file_data_t*
} file_list_t;

file_list_t* file_list_init();
void         file_list_free(file_list_t* fl);

// parses a directory and adds each file to file_list
void file_list_parse_dir(file_list_t* fl);
void file_list_add(file_list_t* fl, file_data_t* f);
void file_list_print(file_list_t* fl);

#endif
