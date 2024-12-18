#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "gap_buffer.h"

int main(_In_ int argc, _In_ char* argv[]) {
  Config* config = NULL;
  bool result = GetConfig(config);

  GapBuffer* buffer = new_buffer(512);
  if(buffer == NULL){
    return -1;
  }

  if(buffer){
    free_buffer(buffer);
  }

  return 0;
}