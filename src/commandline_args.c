#include "commandline_args.h"

commandline_args_t* commandline_args_init(int argc, char** argv, commandline_args_t* args_list, int args_list_count, commandline_args_error_callback_t error_callback)
{
	commandline_args_t* cmd_args = malloc(sizeof(commandline_args_t));

	cmd_args->argc = argc;
	cmd_args->argv = argv;
	cmd_args->error_callback = error_callback;
	cmd_args->args_list = args_list;
	cmd_args->args_list_count = args_list_count;
	
	return cmd_args;
}

void commandline_args_free(commandline_args_t* cmd_args)
{
	free(cmd_args);
}

void parse_arg(commandline_args_t* cmd_args, int idx)
{
   	// check if value is required
   	
   	char* arg_str = "";
   	char* val_str = "";
   	bool  val_specified = false;

	char* argv = cmd_args->argv[idx];
   
   	// get arg_str and val_str out of argv

   	char* equal_sign = strchr(argv, '=');
   	
   	if (equal_sign)
   	{
   		// format: arg=value

   		int equal_sign_pos = (int)(equal_sign - argv);
   		
   		arg_str = malloc(equal_sign_pos + 1);
   		val_str = malloc(strlen(equal_sign + 1) + 1);

   		strcpy(arg_str, argv);
   		strcpy(val_str, equal_sign + 1);

   		arg_str[equal_sign_pos] = '\0';
   		val_str[strlen(equal_sign)] = '\0';

   		val_specified = true;
   	}
   	else
   	{
   		arg_str = malloc(strlen(argv) + 1);
   		strcpy(arg_str, argv);
   		arg_str[strlen(argv)] = '\0';
   	}

	// checks if argv is defined in cmd_args->args_list and adds corresponding info to cmd_args->args_list
	
	for (int i = 0; i < cmd_args->args_list_count; i++)
	{
		// check if arg specified matches 

		commandline_arg_t* arg = &cmd_args->args_list[i];
		
		if (strcmp(arg->arg, arg_str) != 0)
			continue;
	
		// check for equal sign

		if (arg->value_required != val_specified)
			cmd_args->error_callback(commandline_args_parse_error_missing_equal_sign, argv);

		arg->callback(val_str);

		return;
	}
	
	cmd_args->error_callback(commandline_args_parse_error_argument_does_not_exist, argv);

	return;
}

void commandline_args_parse(commandline_args_t* cmd_args)
{
	// start at 1 to ignore executable path

	for (int i = 1; i < cmd_args->argc; i++)
	{
		parse_arg(cmd_args, i);
	}
}
