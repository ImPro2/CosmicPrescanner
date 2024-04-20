#ifndef DEPENDENCY_TREE_H
#define DEPENDENCY_TREE_H

#include "dependency_node.h"
#include "dynamic_list.h"
#include "file_list.h"

typedef struct
{
	dep_node_t* root;
	dynamic_list_t* sorted_files_list; // of type file_data_t;
} dep_tree_t;

dep_tree_t* dep_tree_init(file_list_t* file_list);
void        dep_tree_free(dep_tree_t* tree);

void dep_tree_sort_files(dep_tree_t* tree);
void dep_tree_print(dep_tree_t* tree);

#endif
