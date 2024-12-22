#pragma once
#include <stdbool.h>

typedef struct Config{
    bool default_insert_mode;
    bool use_external_scripts;
} Config;

bool GetConfig(Config** config);
void FreeConfig(Config* config);

