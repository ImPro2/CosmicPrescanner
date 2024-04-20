#ifndef APP_H
#define APP_H

// singleton*

typedef struct
{
	const char* root_path;
} app_data_t;

void app_init(int argc, char** argv);
void app_free();
void app_run();
void app_exit();
app_data_t* get_app_data();

#endif
