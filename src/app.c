#include "app.h"
#include "dynamic_list.h"
#include "file_data.h"
#include "file_list.h"
#include "commandline_args.h"
#include <stdbool.h>

static app_data_t* s_app_data;

static void cmd_arg_help(const char*)
{
	printf("help\n");

	app_exit();
}

static void cmd_arg_path(const char* path)
{
	s_app_data->root_path = path;
}

static void cmd_arg_error(commandline_args_parse_error_t error, const char* argv)
{
   	switch (error)
   	{
   		case commandline_args_parse_error_missing_equal_sign:
   		{
			printf("Error: Argument %s requires a value.\n", argv);
   			break;
   		}
   		case commandline_args_parse_error_argument_does_not_exist:
   		{
			printf("Error: Argument %s does not exist.\n", argv);
   			break;
   		}
   	}
	
	cmd_arg_help("");
}

#define COMMANDLINE_ARGUMENT_COUNT 3

static commandline_arg_t s_commandline_arguments[COMMANDLINE_ARGUMENT_COUNT] = {
	{ "--help", false, &cmd_arg_help },
	{ "-h",     false, &cmd_arg_help },
	{ "--path", true,  &cmd_arg_path }
};

void setup_commandline(int argc, char** argv)
{
	commandline_args_t* cmd_args = commandline_args_init(argc, argv, s_commandline_arguments, COMMANDLINE_ARGUMENT_COUNT, &cmd_arg_error);

	commandline_args_parse(cmd_args);

	cmd_args = NULL;
	commandline_args_free(cmd_args);
}

void app_init(int argc, char** argv)
{
	s_app_data = malloc(sizeof(app_data_t));
	
	setup_commandline(argc, argv);		

	//s_app_data->root_path = root_path;
}

void app_free()
{
	free(s_app_data);
}

void app_exit()
{
	app_free();
	exit(0);
}

app_data_t* get_app_data()
{
	return s_app_data;
}

/*
 * There are 5 steps:
 * 1: Read all the files and put them in a dynamic list. Mark only the root file.
 * 2: Parse each file for its listed dependencies and add this dependency data to each file element in the dynamic list.
 * 3: Construct a tree:
 *    1. Start with root.
 *    2. Add each dependency of element as children of node
 *    3. Loop through each child node and repeat step 2 until there are no more dependencies.
 * 4: Traverse the tree:
 *    1. Start with root.
 *    2. Loop through each child.
 *    3. If child has children, repeat step 2 else add node to new dynamic list if it has not yet been added. 
 * 5: Print dynamic array
 */

void app_run(app_data_t* app)
{
	file_list_t* fl = file_list_init();
	
	// Step 1 and 2

	file_list_parse_dir(fl);

	// TODO: Step 3 and 4
	
	file_list_print(fl);
}


