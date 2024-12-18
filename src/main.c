#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "gap_buffer.h"

int _Success_(return == 0) main(_In_ int argc, _In_ char* argv[]) {
  Config* config = NULL;
  bool result = GetConfig(config);
  if(result == false){
    return -1;
  }

  GapBuffer* buffer = new_buffer(512);
  if(buffer == NULL){
    return -1;
  }

  if(buffer){
    free_buffer(buffer);
  }

  if (config) {
      FreeConfig(config);
  }

  return 0;
}