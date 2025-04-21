#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJ(type, objectType) \
    (type *)allocateObject(sizeof(type), objectType)

static ObjString *allocateString(char *chars, int length);
static Obj *allocateObject(size_t size, ObjType type);

// TODO: optmize two allocations to one.
ObjString *copyString(const char *chars, int length)
{
    char *heapChars = ALLOCATE(char, length + 1);
    memcpy(heapChars, chars, length);
    heapChars[length] = '\0';
    return allocateString(heapChars, length);
}

ObjString *takeString(char *chars, int length)
{
    return allocateString(chars, length);
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

static ObjString *allocateString(char *chars, int length)
{
    ObjString *string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->length = length;
    string->chars = chars;
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