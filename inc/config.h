#pragma once
#include <stdbool.h>

#ifdef __linux__

#elif _WIN32
#include <Windows.h>
#endif

typedef struct Config{
    bool default_insert_mode;
    bool use_external_scripts;
} Config;

#ifdef __linux__
bool GetConfig(Config* config);
void FreeConfig(Config* config);
#elif _WIN32
bool _Success_(return != false) GetConfig(_Out_opt_ Config* config);
void FreeConfig(_In_ Config* config);
#endif

