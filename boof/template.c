/* This template was written for the purpose of transpiling boolf*ck programs
 * into C. It declares data types, implements the memory using dynamic arrays,
 * and even defines macros corresponding to the seven bool*ck instructions.
 * With all that work done, it's very easy for the accompanying Python script
 * to just insert a long list of boolf*ck instructions in main() and feed the
 * resulting program text into a C compiler.
 */

// For fputs, stderr, putchar, getchar, EOF
#include <stdio.h>

// For exit, EXIT_SUCCESS, EXIT FAILURE, malloc, realloc, free
#include <stdlib.h>

// For uint8_t and int64_t
#include <stdint.h>

// For bool
#include <stdbool.h>

// Print an error message and terminate the program
void error(const char * message)
{
    fputs("ERROR: ", stderr);
    fputs(message, stderr);
    fputs("\n", stderr);
    exit(EXIT_FAILURE);
}

// Underlying type to use for the address pointer
typedef int64_t Address;

// Dynamic array of bool values that can be reallocated as needed
typedef struct {
    bool * values;
    size_t capacity;
    size_t count;
} Bools;

// How large should the dynamic bools arrays be when first allocated?
#define BOOLS_INITIAL_CAPACITY 2

// By how much should the dynamic bools arrays expand when they reallocate?
#define BOOLS_REALLOC_FACTOR 2

// Allocate initial heap memory for the dynamic bools array
void bools_initialize(Bools * bools)
{
    bools->values = malloc(sizeof(bool) * BOOLS_INITIAL_CAPACITY);
    if (bools->values == NULL) {
        error("failure to allocate bools array");
    }
    bools->capacity = BOOLS_INITIAL_CAPACITY;
    bools->count = 0;
}

// Deallocate the heap memory being used by the dynamic bools array
void bools_destroy(Bools * bools)
{
    free(bools->values);
    bools->values = NULL;
    bools->count = bools->capacity = 0;
}

// Append a new bool value to the end of the dynamic array of bools
void bools_append(Bools * bools, bool b)
{
    if (bools->capacity <= bools->count) {
        size_t new_capacity = bools->capacity * BOOLS_REALLOC_FACTOR;
        bools->values = realloc(bools->values, sizeof(bool) * new_capacity);
        if (bools->values == NULL) {
            error("failure to reallocate bools array");
        }
        bools->capacity = new_capacity;
    }

    bools->values[(bools->count)++] = b;
}

// Retrieve the bool value stored at a particular index
// If index is out of range, append false bools as needed
bool bools_get(Bools * bools, size_t index)
{
    while (bools->count <= index) {
        bools_append(bools, false);
    }
    return bools->values[index];
}

// Set the bool value stored at a particular index
// If index is out of range, append false bools as needed
void bools_set(Bools * bools, size_t index, bool b)
{
    while (bools->count <= index) {
        bools_append(bools, false);
    }
    bools->values[index] = b;
}

// Struct representing the infinite array of boolf*ck memory cells
// (both positive and negative addresses are supported)
typedef struct {
    Bools positive;
    Bools negative;
} Memory;

// Allocate initial heap memory for the memory struct
void memory_initialize(Memory * memory)
{
    bools_initialize(&memory->positive);
    bools_initialize(&memory->negative);
}

// Deallocate the heap memory being used by the memory struct
void memory_destroy(Memory * memory)
{
    bools_destroy(&memory->positive);
    bools_destroy(&memory->negative);
}

// Retrieve the value of the bool in the current memory cell
bool memory_get(Memory * memory, Address address)
{
    size_t index;
    if (address >= 0) {
        index = address;
        return bools_get(&memory->positive, index);
    } else {  // The address is negative
        index = -(address + 1);
        return bools_get(&memory->negative, index);
    }
}

// Set the bool in the current memory cell to a particular value
void memory_set(Memory * memory, Address address, bool value)
{
    size_t index;
    if (address >= 0) {
        index = address;
        bools_set(&memory->positive, index, value);
    } else {  // The address is negative
        index = -(address + 1);
        bools_set(&memory->negative, index, value);
    }
}

// Increment the bool in the current memory cell by one
void memory_increment(Memory * memory, Address address)
{
    bool value = memory_get(memory, address);
    memory_set(memory, address, !value);
}

// Write one bool from the current memory cell to stdout
void output_bool(Memory * memory, Address address)
{
    static uint8_t buffer = 0;
    static size_t bools_buffered = 0;

    bool b = memory_get(memory, address);
    buffer |= (b ? 1 : 0) << bools_buffered;
    ++bools_buffered;

    if (bools_buffered == 8) {
        int c = buffer;
        putchar(c);
        buffer = 0;
        bools_buffered = 0;
    }
}

// Load one bool into the current memory cell from stdin
void input_bool(Memory * memory, Address address)
{
    static uint8_t buffer = 0;
    static size_t bools_buffered = 0;

    if (bools_buffered == 0) {
        int c = getchar();
        if (c == EOF) {
            c = 0;
        }
        buffer = c;
        bools_buffered = 8;
    }

    bool b = (buffer & (1 << (bools_buffered - 1))) != 0;
    memory_set(memory, address, b);
    --bools_buffered;
}

/* The following macros correspond directly to each of the seven boolf*ck
 * instructions. Given the local variables referenced here, these macros only
 * make sense within the context of the main() function. Note: most of them
 * (but not all of them) *intentionally* include semicolons.
 */
#define MOVE_RIGHT ++address;
#define MOVE_LEFT --address;
#define INCREMENT memory_increment(&memory, address);
#define PUT_BOOL output_bool(&memory, address);
#define GET_BOOL input_bool(&memory, address);
#define LOOP_START while (memory_get(&memory, address)) {
#define LOOP_END }

// Skeleton main() function to be filled with the appropriate instructions
int main()
{
    Address address;
    Memory memory;

    address = 0;
    memory_initialize(&memory);

    /* INSERT INSTRUCTIONS HERE */

    // Flush the output buffer by writing seven more zeros
    LOOP_START
    INCREMENT
    LOOP_END
    PUT_BOOL
    PUT_BOOL
    PUT_BOOL
    PUT_BOOL
    PUT_BOOL
    PUT_BOOL
    PUT_BOOL

    memory_destroy(&memory);
    (void) address;  // Silence warnings about unused variable

    return EXIT_SUCCESS;
}
