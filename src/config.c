#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <pwd.h>

#ifndef MAX_PATH
//This should be okay on modern linux's. I will change it if that is not the case.
#define MAX_PATH 4096
#endif
  
bool GetConfigDir(char** path) {
  char* temp;// = malloc(sizeof(char) * MAX_PATH);
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
  //free(temp);
  return true;
}

bool WriteDefaultConfig(char** path) {
  size_t cfg_len = strlen("/config.cfg");
  size_t difference = strlen(*path) - cfg_len;
  char* temp = malloc(sizeof(char) * (strlen(*path) - cfg_len));
  strncpy(temp, *path, difference);
  temp[difference] = '\0';

  struct stat st = {0};
  if(stat(temp, &st) == -1) {
    mkdir(temp, 0700);
  }

  free(temp);

  FILE* file = fopen(*path, "w");
  if(file == NULL) {
    return false;
  }
  
  fprintf(file, "default_insert_mode=false\n");
  fprintf(file, "use_external_scripts=false\n");

  fclose(file);
  
  return true;
}

bool GetConfig(Config** config) {
  char* config_path = malloc(sizeof(char) * MAX_PATH);
  if(config_path == NULL){
    return false;
  }
  
  bool b = GetConfigDir(&config_path);
  if(b == false){
    free(config_path);
    return false;
  }

  strcat(config_path, "/config.cfg");
  
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
  
  char* token = strtok(data, "\n");
  while(token != NULL){
    if(strncmp(token, "default_insert_mode=", 20) == 0){
      (*config)->default_insert_mode = (strncmp(token + 20, "true", 4) == 0);
    }
    else if(strncmp(token, "use_external_scripts=", 21) == 0){
      (*config)->use_external_scripts = (strncmp(token + 21, "true", 4) == 0);
    }
    token = strtok(NULL, "\n");
  }
  
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
  
  if(*config == NULL) {
    free(config_path);
    return false;
  }
  
  (*config)->default_insert_mode = false;
  (*config)->use_external_scripts = false;
  
  if(config_path != NULL) {
    free(config_path);
  }
  
  return true;
}

void FreeConfig(Config* config) { 
  if(!config) {
      return;
  }

  free(config);
}