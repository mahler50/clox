#include <stdio.h>
#include <stdarg.h>

#include "common.h"
#include "debug.h"
#include "compiler.h"
#include "vm.h"

static InterpretResult run();
static void resetStack();
static void runtimeError(const char *format, ...);
static bool isFalsey(Value value);

VM vm;

void initVM()
{
    resetStack();
}

void freeVM()
{
}

// Enterence of interpretor.
InterpretResult interpret(const char *source)
{
    Chunk chunk;
    initChunk(&chunk);

    // Compile source code to byte chunks.
    if (!compile(source, &chunk))
    {
        freeChunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }

    vm.chunk = &chunk;
    vm.ip = vm.chunk->code;

    InterpretResult result = run();
    freeChunk(&chunk);
    return result;
}

// Push a value to the top of stack.
void push(Value value)
{
    *vm.stackTop = value;
    vm.stackTop++;
}

// Pop off a value from the top of stack.
Value pop()
{
    vm.stackTop--;
    return *vm.stackTop;
}

// Get the peek element's pointer with specfic distance, do not pop any value.
Value *peek(int distance)
{
    return &vm.stackTop[-1 - distance];
}

// Compares two Values whether they are totaly equal.
bool valuesEqual(Value a, Value b)
{
    if (a.type != b.type)
        return false;
    switch (a.type)
    {
    case VAL_BOOL:
        return AS_BOOL(a) == AS_BOOL(b);
    case VAL_NIL:
        return true;
    case VAL_NUMBER:
        return AS_NUMBER(a) == AS_NUMBER(b);
    default:
        return false; // Unreachable.
    }
}

static InterpretResult run()
{
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(value_type, op)                         \
    do                                                    \
    {                                                     \
        if (!IS_NUMBER(*peek(0)) || !IS_NUMBER(*peek(1))) \
        {                                                 \
            runtimeError("Operands must be number.");     \
            return INTERPRET_RUNTIME_ERROR;               \
        }                                                 \
        double b = AS_NUMBER(pop());                      \
        double *a = &AS_NUMBER(*peek(0));                 \
        *a = *a op b;                                     \
    } while (false)

    for (;;)
    {
#ifdef DEBUG_TRACE_EXECUTION
        printf("        ");
        for (Value *slot = vm.stack; slot < vm.stackTop; slot++)
        {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n");
        disassembleInstruction(vm.chunk,
                               (int)(vm.ip - vm.chunk->code));
#endif
        uint8_t instruction;
        switch (instruction = READ_BYTE())
        {
        case OP_CONSTANT:
        {
            Value constant = READ_CONSTANT();
            push(constant);
            printValue(constant);
            printf("\n");
            break;
        }
        case OP_NIL:
            push(NIL_VAL);
            break;
        case OP_TRUE:
            push(BOOL_VAL(true));
            break;
        case OP_FALSE:
            push(BOOL_VAL(false));
            break;
        case OP_EQUAL:
            Value a = pop();
            Value b = pop();
            push(BOOL_VAL(valuesEqual(a, b)));
            break;
        case OP_GREATER:
            BINARY_OP(BOOL_VAL, >);
            break;
        case OP_LESS:
            BINARY_OP(BOOL_VAL, <);
            break;
        case OP_ADD:
            BINARY_OP(NUMBER_VAL, +);
            break;
        case OP_SUBTRACT:
            BINARY_OP(NUMBER_VAL, -);
            break;
        case OP_MULTIPLY:
            BINARY_OP(NUMBER_VAL, *);
            break;
        case OP_DIVIDE:
            BINARY_OP(NUMBER_VAL, /);
            break;
        case OP_NOT:
            *peek(0) = BOOL_VAL(isFalsey(*peek(0)));
            break;
        case OP_NEGATE:
            if (!IS_NUMBER(*peek(0)))
            {
                runtimeError("Operand must be a number");
                return INTERPRET_RUNTIME_ERROR;
            }
            *peek(0) = NUMBER_VAL(-(AS_NUMBER(*peek(0))));
            break;
        case OP_RETURN:
        {
            printValue(pop());
            printf("\n");
            return INTERPRET_OK;
        }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

static void resetStack()
{
    vm.stackTop = vm.stack;
}

static void runtimeError(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    size_t instruction = vm.ip - vm.chunk->code - 1;
    int line = getLine(&vm.chunk->lines, (int)instruction);
    fprintf(stderr, "[line %d] in script\n", line);
    resetStack();
}

// If Value is `nil` or `false`, return true.
static bool isFalsey(Value value)
{
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}