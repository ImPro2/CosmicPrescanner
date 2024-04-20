#include "dependency_node.h"

dep_node_t* dep_node_init(file_data_t* file_data, dep_node_t* parent)
{
	dep_node_t* node = malloc(sizeof(dep_node_t));

	node->file_data = file_data;
	node->parent    = parent;

	node->children_list = dynamic_list_init(3, sizeof(dep_node_t));

	return node;
}

void dep_node_free(dep_node_t* node)
{
	// Traverse & Recurse
	
	for (int i = 0; i < node->children_list->count; i++)
		dep_node_free(node->children_list->elements[i]);

	free(node);
	node = NULL;
}

void dep_node_add(dep_node_t* node, dep_node_t* child)
{
	dynamic_list_add(node->children_list, child);
}

