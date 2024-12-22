#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define MIN_BUF_SIZE 128

/*
This specific implementation was stolen from a blog post that i do not remember. I will find it and give credit where it is due.
*/

typedef struct GapBuffer {
    size_t size;
    size_t cursor;
    size_t gap_end;
    char* buffer;
} GapBuffer;

GapBuffer* new_buffer(size_t init_size);
void free_buffer(GapBuffer* buffer);

#define gb_front(buf)   ((buf)->cursor)
#define gb_back(buf)    ((buf)->size - (buf)->gap_end)
#define gb_used(buf)    (gb_front(buf) + gb_back(buf))

void move_backtext(GapBuffer* buffer, char* new_buffer, size_t new_size);
void shrink_buffer(GapBuffer* buffer, size_t new_size);
bool grow_buffer(GapBuffer* buffer, size_t new_size);

#define capped_dbl_size(S) ((S) < SIZE_MAX / 2) ? (2 * (S)) : SIZE_MAX

bool insert_character(GapBuffer* buffer, char c);
void cursor_right(GapBuffer* buffer);
void cursor_left(GapBuffer* buffer);
void backspace(GapBuffer* buffer);

void delete(GapBuffer* buffer);

char* extract_text(GapBuffer* buffer);

void print_buffer(GapBuffer* buffer);
