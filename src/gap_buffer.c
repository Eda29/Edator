#include "gap_buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __linux__
#include <sys/param.h>
#elif
//Windows implementation and headers
#endif

GapBuffer* new_buffer(size_t init_size){
    GapBuffer* buffer = malloc(sizeof(*buffer));
    if(!buffer) {
        return NULL;
    }

    init_size = MAX(init_size, MIN_BUF_SIZE);
    buffer->buffer = malloc(init_size);
    if(!buffer->buffer){
        free(buffer);
        return NULL;
    }

    buffer->size = init_size;
    buffer->cursor = 0;
    buffer->gap_end = init_size;
    return buffer;
}

void free_buffer(GapBuffer* buffer){
    if(!buffer){
        return;
    }

    free(buffer->buffer);
    free(buffer);
}

void move_backtext(GapBuffer* buffer, char* new_buffer, size_t new_size){
    memmove(new_buffer + new_size - gb_back(buffer),
        buffer->buffer + buffer->gap_end,
        gb_back(buffer));
}

void shrink_buffer(GapBuffer* buffer, size_t new_size){
    new_size = MAX(new_size, MIN_BUF_SIZE);
    if(new_size < gb_used(buffer)){
        return;
    }

    move_backtext(buffer, buffer->buffer, new_size);
    buffer->gap_end = new_size - gb_back(buffer);
    buffer->size = new_size;

    char* new_buffer = realloc(buffer->buffer, new_size);
    if(new_buffer){
        buffer->buffer = new_buffer;
    }
}

bool grow_buffer(GapBuffer* buffer, size_t new_size){
    new_size = MAX(new_size, MIN_BUF_SIZE);
    if(buffer->size >= new_size){
        return false;
    }

    char* new_buffer = realloc(buffer->buffer, new_size);
    if(!new_buffer){
        return false;
    }

    move_backtext(buffer, new_buffer, new_size);
    buffer->buffer = new_buffer;
    buffer->gap_end = new_size - gb_back(buffer);
    buffer->size = new_size;

    return true;
}

bool insert_character(GapBuffer* buffer, char c){
    if(buffer->cursor == buffer->gap_end){
        size_t new_size = capped_dbl_size(buffer->size);
        if(!grow_buffer(buffer, new_size)){
            return false;
        }
    }
    buffer->buffer[buffer->cursor++] = c;
    return true;
}

void cursor_left(GapBuffer* buffer){
    if(buffer->cursor > 0){
        buffer->buffer[--buffer->gap_end] = buffer->buffer[--buffer->cursor];
    }
}

void cursor_right(GapBuffer* buffer){
    if(buffer->gap_end < buffer->size){
        buffer->buffer[buffer->cursor++] = buffer->buffer[buffer->gap_end++];
    }
}

void backspace(GapBuffer* buffer){
    if(buffer->cursor > 0){
        buffer->cursor--;
    }

    if(gb_used(buffer) < buffer->size / 4){
        shrink_buffer(buffer, buffer->size / 2);
    }
}

void delete(GapBuffer* buffer){
    if(buffer->gap_end < buffer->size){
        buffer->gap_end++;
    }

    if(gb_used(buffer) < buffer->size / 4){
        shrink_buffer(buffer, buffer->size / 2);
    }
}

char* extract_text(GapBuffer* buffer){
    if(SIZE_MAX == gb_used(buffer)){
        return NULL;
    }

    char* text = malloc(gb_used(buffer) + 1);
    if(!text){
        return NULL;
    }

    strncpy(text, buffer->buffer, buffer->cursor);
    strncpy(text + buffer->cursor, buffer->buffer + buffer->gap_end, gb_back(buffer));
    text[gb_used(buffer)] = '\0';
    return text;
}

void print_buffer(GapBuffer* buffer){
    char* text = extract_text(buffer);
    printf("%s\n", text);
    free(text);
}