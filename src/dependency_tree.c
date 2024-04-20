#include "dependency_tree.h"
#include "file_data.h"
#include "dynamic_list.h"
#include <stdbool.h>

// in init
static dynamic_list_t* s_file_data_already_parsed_list; // of type file_data_t
static bool s_file_data_already_parsed = false;

void add_dep_recurse(dep_node_t* node)
{
	for (int i = 0; i < node->file_data->dependency_list->count; i++)
	{
		file_data_t* f = ((file_dependency_data_t*)node->file_data->dependency_list->elements[i])->file_data;
		dep_node_t* child = dep_node_init(f, node);
		dep_node_add(node, child);

		if (child->file_data->dependency_list->count > 0)
		{
			if (!dynamic_list_contains(s_file_data_already_parsed_list, child->file_data))
			{
				dynamic_list_add(s_file_data_already_parsed_list, child->file_data);
				add_dep_recurse(child);
			}
			else
			{
				//
			}
		}
	}
}

void init_tree_from_file_list(dep_tree_t* tree, file_list_t* file_list)
{
	// set the root node to be the first element of the file list.
	tree->root = dep_node_init(((file_data_t*)file_list->file_list->elements[0]), NULL);
	add_dep_recurse(tree->root);
}

dep_tree_t* dep_tree_init(file_list_t* file_list)
{
	if (!s_file_data_already_parsed)
	{
		s_file_data_already_parsed_list = dynamic_list_init(50, sizeof(file_data_t));
		s_file_data_already_parsed = true;
	}

	dep_tree_t* tree = malloc(sizeof(dep_tree_t));

	tree->sorted_files_list = dynamic_list_init(50, sizeof(file_data_t));
	init_tree_from_file_list(tree, file_list);

	return tree;
}

void dep_tree_free(dep_tree_t* tree)
{
}

void dep_tree_sort_files_recurse(dep_tree_t* tree, dep_node_t* node)
{
	for (int i = 0; i < node->children_list->count; i++)
	{
		dep_node_t* child = (dep_node_t*)node->children_list->elements[i];

		if (child->children_list->count > 0)
			dep_tree_sort_files_recurse(tree, child);

		if (!dynamic_list_contains(tree->sorted_files_list, child->file_data))
			dynamic_list_add(tree->sorted_files_list, child->file_data);
	}
}

void dep_tree_sort_files(dep_tree_t* tree)
{
	dep_tree_sort_files_recurse(tree, tree->root);
}

void dep_tree_print_recurse(dep_node_t* node, int indent_count)
{
	for (int i = 0; i < indent_count; i++)
		printf("\t");

	printf("- %s: children count %i:\n", node->file_data->module_name, node->children_list->count);

	for (int i = 0; i < node->children_list->count; i++)
	{
		if (node->children_list->count > 0)
			dep_tree_print_recurse(node->children_list->elements[i], indent_count + 1);
	}
}

void dep_tree_print(dep_tree_t* tree)
{
	//dep_tree_print_recurse(tree->root, 0);

	for (int i = 0; i < tree->sorted_files_list->count; i++)
	{
		file_data_t* file = (file_data_t*)tree->sorted_files_list->elements[i];
		file_data_print(file);
	}
}
