#ifndef clox_table_h
#define clox_table_h

#include "common.h"
#include "value.h"

#define TABLE_MAX_LOAD 0.75

typedef struct
{
    ObjString *key;
    Value value;
} Entry;

typedef struct
{
    int count;
    int capacity;
    Entry *entries;

} Table;

void initTable(Table *tabel);
void freeTable(Table *table);
// Set a key-value into hashtable, return true is this key is new.
bool tableSet(Table *table, ObjString *key, Value value);
// Add all entries from src to dest.
void tableAddAll(Table *src, Table *dest);
// Retrive value from table with specfic key.
bool tableGet(Table *table, ObjString *key, Value *value);
bool tableDelete(Table *table, ObjString *key);
ObjString *tableFindString(Table *table, const char *chars, int length, uint32_t hash);

#endif