#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynamic_list.h"
#include "file_data.h"
#include "file_list.h"
#include "app.h"

int main(int argc, char** argv)
{
	app_init(argc, argv);
	app_run();
	app_free();
	
	return 0;
}
