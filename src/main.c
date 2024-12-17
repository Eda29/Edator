#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __linux__
  #include <unistd.h>
  bool GetConfigDir(const char* path){
    //Implement this.
    return false;
  }
#elif _WIN32
  #include <ShlObj.h>
  #include <KnownFolders.h>
  #include <Windows.h>

  bool GetConfigDir(char* path){
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


int main(int argc, char* argv[]) {
  char configPath[MAX_PATH];
  bool b = GetConfigDir(&configPath[0]);
  if(b == false){
    return -1;
  }



  return 0;
}