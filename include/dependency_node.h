#ifndef DEPENDENCY_NODE_H
#define DEPENDENCY_NODE_H

//#include "dependency_tree.h"
#include "dynamic_list.h"
#include "file_data.h"

typedef struct dep_node_t
{
	file_data_t* file_data;
	dynamic_list_t* children_list; // of type dep_node_t
	struct dep_node_t* parent;
} dep_node_t;

dep_node_t* dep_node_init(file_data_t* file_data, dep_node_t* parent);
void        dep_node_free(dep_node_t* node);

void dep_node_add(dep_node_t* node, dep_node_t* child);

#endif
