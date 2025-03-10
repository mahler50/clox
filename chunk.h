#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef enum
{
    OP_CONSTANT,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NEGATE,
    OP_RETURN,
} OpCode;

typedef struct
{
    // current line
    int line;
    // inline offset
    int offset;
} Line;

typedef struct
{
    int count;
    int capacity;
    Line *line;
} Lines;

typedef struct
{
    int count;
    int capacity;
    uint8_t *code;
    Lines lines;
    // int *lines;
    ValueArray constants;
} Chunk;

void initChunk(Chunk *chunk);
void freeChunk(Chunk *chunk);
void writeChunk(Chunk *chunk, uint8_t byte, int line);
int addConstant(Chunk *chunk, Value value);

void initLines(Lines *lines);
void freeLines(Lines *lines);
void writeLines(Lines *lines, int line);
int getLine(Lines *lines, int offset);
#endif