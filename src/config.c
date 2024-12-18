#include "config.h"
#include <stdio.h>
#include <string.h>

#ifdef __linux__
  #include <unistd.h>
  bool GetConfigDir(_Out_ const char* path){
    //Implement this.
    return false;
  }
#elif _WIN32
    #include <ShlObj.h>
    #include <KnownFolders.h>
    #include <Windows.h>

    bool GetConfigDir(_Out_ char* path) {
        PWSTR temp;
    
        HRESULT result = SHGetKnownFolderPath(&FOLDERID_LocalAppData, 0, NULL, &temp);
        if(FAILED(result)){
          CoTaskMemFree(temp);
          puts("Failure to get program data");
          return false;
        }   
    
        //convert the wchar array to char*
        wcstombs(path, temp, MAX_PATH);
        CoTaskMemFree(temp);    
    
        //Append the editors' specific path
        const char* localPath = "\\edator\\config.cfg";
        strcat(path, localPath);    
    
        return true;
    }

#endif

bool GetConfig(_Out_ Config* config) {
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
    
    fread(data, f_size, 1, fp_Config);
    if(ferror(fp_Config) != 0){
      free(data);
      fclose(fp_Config);
      return false;
    }
    if(feof(fp_Config) != 0){
      free(data);
      fclose(fp_Config);
      return false;
    }

    fclose(fp_Config);

    config = malloc(sizeof(config));
    if(config == NULL){
        free(data);
        return false;
    }
    
    //Parse the text file into the config object.

    free(data);


    return true;
}

void FreeConfig(Config* config) {
    if(!config){
        return;
    }

    free(config);
}