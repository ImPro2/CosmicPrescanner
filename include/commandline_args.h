#ifndef COMMANDLINE_ARGS_H
#define COMMANDLINE_ARGS_H
#include "dynamic_list.h"
#include <stdbool.h>

typedef enum
{
	commandline_args_parse_error_none,
	commandline_args_parse_error_missing_equal_sign,
	commandline_args_parse_error_argument_does_not_exist,
} commandline_args_parse_error_t;

typedef void(*commandline_arg_callback_t)(const char*);
typedef void(*commandline_args_error_callback_t)(commandline_args_parse_error_t, const char* argv);

typedef struct
{
	const char* arg;
	bool value_required;
	
	commandline_arg_callback_t callback;
} commandline_arg_t;

typedef struct
{
	int argc;
	char** argv;

	commandline_args_error_callback_t error_callback;

	commandline_arg_t* args_list;
	int args_list_count;
} commandline_args_t;

commandline_args_t* commandline_args_init(int argc, char** argv, commandline_args_t* args_list, int args_list_count, commandline_args_error_callback_t error_callback);
void                commandline_args_free(commandline_args_t* cmd_args);

void commandline_args_parse(commandline_args_t* cmd_args);

#endif
