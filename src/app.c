#include "app.h"
#include "dynamic_list.h"
#include "file_data.h"
#include "file_list.h"
#include "commandline_args.h"
#include "dependency_tree.h"
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

static commandline_arg_t s_commandline_arguments[] = {
	{ "--help", false, &cmd_arg_help },
	{ "-h",     false, &cmd_arg_help },
	{ "--path", true,  &cmd_arg_path },
};

static int s_commandline_argument_count = (int)sizeof(s_commandline_arguments) / (int)sizeof(commandline_arg_t);

void setup_commandline(int argc, char** argv)
{
	commandline_args_t* cmd_args = commandline_args_init(argc, argv, s_commandline_arguments, s_commandline_argument_count, &cmd_arg_error);

	commandline_args_parse(cmd_args);

	cmd_args = NULL;
	commandline_args_free(cmd_args);
}

void app_init(int argc, char** argv)
{
	s_app_data = malloc(sizeof(app_data_t));
	
	setup_commandline(argc, argv);		
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

void app_run(app_data_t* app)
{
	file_list_t* fl = file_list_init();
	file_list_parse_dir(fl);

	dep_tree_t* tree = dep_tree_init(fl);
	dep_tree_sort_files(tree);
	dep_tree_print(tree);
}


