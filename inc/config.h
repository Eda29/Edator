#pragma once
#include <stdbool.h>
#include <Windows.h>

typedef struct Config{
    bool default_insert_mode;
    bool use_external_scripts;
} Config;

bool GetConfig(_Out_ Config* config);
void FreeConfig(_In_ Config* config);

