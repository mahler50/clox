#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"

// Argument `flex` is a choice to support struct with flexible array members.
// If flexible array members do not exist, please set this value as `0`.
#define ALLOCATE_OBJ(type, objectType, flex) \
    (type *)allocateObject(sizeof(type) + flex + 1, objectType)

static ObjString *allocateString(const char *chars, int length);
static Obj *allocateObject(size_t size, ObjType type);

ObjString *copyString(const char *chars, int length)
{
    return allocateString(chars, length);
}

ObjString *takeString(char *chars, int length)
{
    ObjString *string = allocateString(chars, length);
    // free original string
    FREE_ARRAY(char, chars, length + 1);
    return string;
}

void printObj(Value value)
{
    switch (OBJ_TYPE(value))
    {
    case OBJ_STRING:
        printf("%s", AS_CSTRING(value));
        break;
    }
}

static ObjString *allocateString(const char *chars, int length)
{
    ObjString *string = ALLOCATE_OBJ(ObjString, OBJ_STRING, length);
    string->length = length;
    // directly copy to the flexible array.
    memcpy(string->chars, chars, length);
    string->chars[length] = '\0';
    return string;
}

// Allocate a specfic type of Obj.
static Obj *allocateObject(size_t size, ObjType type)
{
    Obj *obj = (Obj *)reallocate(NULL, 0, size);
    obj->type = type;

    // Push front into list.
    obj->next = vm.objects;
    vm.objects = obj;
    return obj;
}