#include "text_buffer.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_BUFFER_SIZE (1000)

bool CreateEmptyTextBuffer(TextBuffer **buffer) {
    TextBuffer* b = *buffer;
    if(b == NULL){
        b = malloc(sizeof(TextBuffer));
        if(b == NULL){
            puts("Buffer was null, and couldn't re-alloc");
            return false;
        }
    }

    b->buffer = malloc(sizeof(unsigned char) * MIN_BUFFER_SIZE);
    if(b->buffer == NULL){
        puts("Failed to allocate memory for buffer");
        free(b);
        return false;
    }

    b->size = MIN_BUFFER_SIZE;
    return true;
}

bool CreateTextBufferFromFile(TextBuffer** buffer, const char* filepath) {
    bool result = CreateEmptyTextBuffer(buffer);
    TextBuffer* buf = *buffer;
    if(result == true) {
        FILE* file = fopen(filepath, "r");
        if(file == NULL){
            fprintf(stdout, "Failed to open file %s\n", filepath);
            result == false;
            return result;
        }

        fseek(file, 0, SEEK_END);
        size_t size = ftell(file);
        fseek(file, 0, SEEK_SET);

        if(buf == NULL){
            puts("Buffer null");
        }
        if(buf->size >= size) {
            size_t num_chunks = 1;
            //start reading the file into the buffer
            int chunks_read = fread(buf->buffer, size, num_chunks, file);
            if(chunks_read != num_chunks){
                //issue
                fprintf(stdout, "Failed to read the requested amount of bytes from file %s", filepath);
                result = false;
            }

            //for now, lets print the buffer to confirm operation
            fprintf(stdout, "%s\n", buf->buffer);
        }

        fclose(file);
    }
    return result;
}

void DestroyTextBuffer(TextBuffer* buffer) {
    if(buffer != NULL) {
        if(buffer->buffer != NULL) {
            free(buffer->buffer);
        }
    }
}