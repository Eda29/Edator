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
  
  bool GetConfigDir(char* path){
    if((path = getenv("XDG_CONFIG_HOME")) == NULL) {
      if((path = getenv("HOME")) == NULL){
        path = getpwuid(getuid())->pw_dir;
      }
    }

    strcat(path, "/edator/config.cfg");

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
        const char* localPath = "\\edator\\config.cfg";
        strcat(path, localPath);    
    
        return true;
    }

#endif

#ifdef __linux__
bool GetConfig(Config* config) {
#elif _WIN32
bool _Success_(return != false) GetConfig(_Out_opt_ Config* config) {
#endif
    char config_path[MAX_PATH];
    bool b = GetConfigDir(&config_path[0]);
    if(b == false){
      return false;
    }

    FILE* fp_Config = fopen(config_path, "r");
    if (fp_Config == NULL) {
        return false;
    }

    fseek(fp_Config, 0, SEEK_END);
    size_t f_size = ftell(fp_Config);
    fseek(fp_Config, 0, SEEK_SET);

    char* data = malloc(sizeof(char) * f_size);
    if(data == NULL){
      return false;
    }
    
    int read_chunks = fread(data, f_size, 1, fp_Config);
    if(ferror(fp_Config) != 0 || read_chunks == 0){
      free(data);
      fclose(fp_Config);
      return false;
    }

    fclose(fp_Config);

    data[f_size-1] = '\0';

    config = malloc(sizeof(Config));
    if(config == NULL){
        free(data);
        return false;
    }
    
    //Parse the text file into the config object.
    puts(data);

    free(data);

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