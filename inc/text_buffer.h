#pragma once
#include <uchar.h>
#include <stdbool.h>

typedef struct _TextBuffer {
    unsigned char* buffer;
    size_t lineCount;
    size_t size;
} TextBuffer;

bool CreateEmptyTextBuffer(TextBuffer** buffer);
bool CreateTextBufferFromFile(TextBuffer** buffer, const char* filepath);

bool WriteTextBuffer(TextBuffer** buffer);

void DestroyTextBuffer(TextBuffer*);