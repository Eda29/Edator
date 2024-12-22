#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __linux__
  #include <unistd.h>
  #include <limits.h>
  #include <sys/types.h>
  #include <pwd.h>

  #ifndef MAX_PATH
  //This should be okay on modern linux's. I will change it if that is not the case.
  #define MAX_PATH 4096
  #endif
  
  bool GetConfigDir(char** path){
    char* temp = malloc(sizeof(char) * MAX_PATH);
    if((temp = getenv("XDG_CONFIG_HOME")) == NULL) {
      if((temp = getenv("HOME")) == NULL){
        temp = getpwuid(getuid())->pw_dir;
      }
      else{
        //append the .config directory if this is the case.
        strcat(temp, "/.config");
      }
    }

    strcat(temp, "/edator");
    printf("Config Path: %s\n", temp);

    strcpy(*path, temp);

    return true;
  }
#elif _WIN32
    #include <ShlObj.h>
    #include <KnownFolders.h>
    #include <Windows.h>

    bool _Success_(return != false) GetConfigDir(_Out_ char* path) {
        WCHAR* temp;
    
        HRESULT result = SHGetKnownFolderPath(&FOLDERID_LocalAppData, 0, NULL, &temp);
        if(FAILED(result)){
          CoTaskMemFree(temp);
          puts("Failure to get program data");
          return false;
        }   
    
        //convert the wchar array to char*
        wcstombs(path, temp, wcslen(temp));
        CoTaskMemFree(temp);    
    
        //Append the editors' specific path
        const char* localPath = "\\edator";
        strcat(path, localPath);    
    
        return true;
    }

#endif

#ifdef __linux__
#include <sys/types.h>
#include <sys/stat.h>
bool WriteDefaultConfig(char** path) {
    struct stat st = {0};
    if(stat((*path), &st) == -1) {
      mkdir((*path), 0700);
    }    

#elif _WIN32
bool WriteDefaultConfig(_In_ char** path) {

#endif
    char* filepath = malloc(sizeof(char) * (strlen(*path) + strlen("/config.cfg")));
    if(filepath == NULL){
      return false;
    }

    strcat(filepath, *path);
    #ifdef __linux__
      strcat(filepath, "/config.cfg");
    #elif _WIN32
      //fucking windows man...
      strcat(filepath, "\\config.cfg");
    #endif

    FILE* file = fopen(filepath, "w");
    if(file == NULL) {
      free(filepath);
      return false;
    }

    free(filepath);

    Config config;
    config.default_insert_mode = false;
    config.use_external_scripts = false;

    int written = fwrite(&config, sizeof config, 1, file);
    //TODO: add check for this!

    fclose(file);

    return true;
}

#ifdef __linux__
bool GetConfig(Config** config) {
#elif _WIN32
bool _Success_(return != false) GetConfig(_Out_opt_ Config* config) {
#endif
    char* config_path = malloc(sizeof(char) * MAX_PATH);
    if(config_path == NULL){
      return false;
    }

    bool b = GetConfigDir(&config_path);
    if(b == false){
      free(config_path);
      return false;
    }

    FILE* fp_Config = fopen(config_path, "r");
    if (fp_Config == NULL) {
        goto New_CFG;
        return false;
    }

    fseek(fp_Config, 0, SEEK_END);
    size_t f_size = ftell(fp_Config);
    fseek(fp_Config, 0, SEEK_SET);

    char* data = malloc(sizeof(char) * f_size);
    if(data == NULL){
      goto New_CFG;
      return false;
    }
    
    size_t read_chunks = fread(data, f_size, 1, fp_Config);
    if(ferror(fp_Config) != 0 || read_chunks == 0){
      free(data);
      fclose(fp_Config);
      goto New_CFG;
      return false;
    }

    fclose(fp_Config);

    data[f_size-1] = '\0';

    *config = malloc(sizeof(Config));
    if(*config == NULL){
        free(data);
        return false;
    }
    
    //Parse the text file into the config object.
    puts(data);

    free(data);
    free(config_path);
    return true;

New_CFG:
    bool wrote_new_cfg = WriteDefaultConfig(&config_path);
    if(!wrote_new_cfg){
      puts("Failed to write config file to path!");
      free(config_path);
    }

    *config = malloc(sizeof(Config));
    if(*config == NULL){
      free(config_path);
      return false;
    }

    (*config)->default_insert_mode = false;
    (*config)->use_external_scripts = false;

    if(config_path != NULL)
      free(config_path);

    return true;
}

#ifdef __linux__
void FreeConfig(Config* config) { 
#elif _WIN32
void FreeConfig(_In_ Config* config) {
#endif
    if(!config){
        return;
    }

    free(config);
}