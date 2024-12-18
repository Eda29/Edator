#pragma once
#include <stdbool.h>
#include <Windows.h>

typedef struct Config{
    bool default_insert_mode;
    bool use_external_scripts;
} Config;

bool _Success_(return != false) GetConfig(_Out_opt_ Config * config);
void FreeConfig(_In_ Config* config);

