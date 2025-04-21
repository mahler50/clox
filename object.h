#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "value.h"

// Get Obj's type.
#define OBJ_TYPE(value) (AS_OBJ(value)->type)

#define IS_STRING(value) isObjType(value, OBJ_STRING)

#define AS_STRING(value) ((ObjString *)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString *)AS_OBJ(value))->chars)

typedef enum
{
    OBJ_STRING,
} ObjType;

// In this way, we can convert Obj's ptr into any specfic
// Obj implement's ptr.
struct Obj
{
    ObjType type;
    struct Obj *next;
};

struct ObjString
{
    Obj obj;
    int length;
    char chars[]; // use flexible array
};

ObjString *copyString(const char *chars, int length);
ObjString *takeString(char *chars, int length);
void printObj(Value value);

static inline bool isObjType(Value value, ObjType type)
{
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif