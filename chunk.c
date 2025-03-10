#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

// init a array of chunks.
void initChunk(Chunk *chunk)
{
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    initLines(&chunk->lines);
    initValueArray(&chunk->constants);
}

void writeChunk(Chunk *chunk, uint8_t byte, int line)
{
    // If count + 1 is gretter than capacity, we resize the array.
    if (chunk->count + 1 > chunk->capacity)
    {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    }

    writeLines(&chunk->lines, line);
    chunk->code[chunk->count] = byte;
    chunk->count++;
}

void freeChunk(Chunk *chunk)
{
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    freeValueArray(&chunk->constants);
    freeLines(&chunk->lines);
    initChunk(chunk);
}

int addConstant(Chunk *chunk, Value value)
{
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}

void initLines(Lines *lines)
{
    lines->count = 0;
    lines->capacity = 0;
    lines->line = NULL;
}

void freeLines(Lines *lines)
{
    FREE_ARRAY(Line, lines->line, lines->capacity);
    initLines(lines);
}

void writeLines(Lines *lines, int line)
{
    // If count + 1 is gretter than capacity, we resize the array.
    if (lines->count + 1 > lines->capacity)
    {
        int oldCapacity = lines->capacity;
        lines->capacity = GROW_CAPACITY(oldCapacity);
        lines->line = GROW_ARRAY(Line, lines->line, oldCapacity, lines->capacity);
    }

    // If it is the first time to write or the line number is different with the previous one,
    // we just insert a new Line.
    if (&lines->line[lines->count] == NULL ||
        lines->line[lines->count].line != line)
    {
        lines->line[lines->count].line = line;
        lines->count++;
    }
    lines->line[lines->count].offset++;
}

// Get chunk's line from chunk's offset.
int getLine(Lines *lines, int offset)
{
    for (int i = 0; i < lines->count; i++)
    {
        if (offset - lines->line[i].offset <= 0)
        {
            return lines->line[i].line;
        }
        offset -= lines->line[i].offset;
    }
}