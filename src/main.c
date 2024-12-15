#ifdef __linux__
  #include <errno.h>
  #include <unistd.h>
  #define CONFIG_PATH "~/.config/edator/config.dat"
#endif

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "text_buffer.h"

TextBuffer* textBuffer = NULL;

int main(int argc, char* argv[]) {
  textBuffer = malloc(sizeof(TextBuffer));
  if(textBuffer == NULL){
    puts("Failed to allocate text buffer object");
    return -1;
  }
  if(argc < 2) {
    //Create an empty file buffer.
    bool success = CreateEmptyTextBuffer(&textBuffer);
    if(success == false) {
      puts("Empty text buffer object creation failed.");
      return -1;
    }
  }
  else {
    puts("File provided");
    //Check if this file exists, and if it does, load it and allow editing on it.
    //if it does not exist, create it with an empty buffer.
    bool success;
    if(access(argv[1], F_OK) == -1){
      //Create an empty buffer and create the file
      switch(errno){
        case ENOENT: {
          success = CreateEmptyTextBuffer(&textBuffer);
          if(success == false) {
            return -1;
          }
        }
      }
    }
    else {
      success = CreateTextBufferFromFile(&textBuffer, argv[1]);
      if(success == false) {
        return -1;
      }
    }
  }

  //now the main editing loop. draw the buffer to the console if it has changed,
  //process any input
  //modify the buffer as needed
  //cleanly exit.
  while(true){
      
  }

Quit:
  puts("Quit.");
  DestroyTextBuffer(textBuffer);
  free(textBuffer);
  return 0;
}