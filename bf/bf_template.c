/* This template was written for the purpose of transpiling Brainf*ck programs
 * into C. It declares data types, implements the memory using dynamic arrays,
 * and even defines macros corresponding to the eight Brainf*ck instructions.
 * With all that work done, it's very easy for the accompanying Python script
 * to just insert a long list of Brainf*ck instructions in main() and feed the
 * resulting program text into a C compiler.
 */

// For fputs, stderr, putchar, getchar, EOF
#include <stdio.h>

// For exit, EXIT_SUCCESS, EXIT FAILURE, malloc, realloc, free
#include <stdlib.h>

// For uint8_t and int64_t
#include <stdint.h>

// Print an error message and terminate the program
void error(const char * message)
{
    fputs("ERROR: ", stderr);
    fputs(message, stderr);
    fputs("\n", stderr);
    exit(EXIT_FAILURE);
}

// Underlying types to use for the Brainf*ck memory cells and address pointer
typedef uint8_t Byte;
typedef int64_t Address;

// Dynamic array of byte values that can be reallocated as needed
typedef struct {
    Byte * values;
    size_t capacity;
    size_t count;
} Bytes;

// How large should the dynamic bytes arrays be when first allocated?
#define BYTES_INITIAL_CAPACITY 2

// By how much should the dynamic bytes arrays expand when they reallocate?
#define BYTES_REALLOC_FACTOR 2

// Allocate initial heap memory for the dynamic bytes array
void bytes_initialize(Bytes * bytes)
{
    bytes->values = malloc(sizeof(Byte) * BYTES_INITIAL_CAPACITY);
    if (bytes->values == NULL) {
        error("failure to allocate bytes array");
    }
    bytes->capacity = BYTES_INITIAL_CAPACITY;
    bytes->count = 0;
}

// Deallocate the heap memory being used by the dynamic bytes array
void bytes_destroy(Bytes * bytes)
{
    free(bytes->values);
    bytes->values = NULL;
    bytes->count = bytes->capacity = 0;
}

// Append a new byte value to the end of the dynamic array of bytes
void bytes_append(Bytes * bytes, Byte byte)
{
    if (bytes->capacity <= bytes->count) {
        size_t new_capacity = bytes->capacity * BYTES_REALLOC_FACTOR;
        bytes->values = realloc(bytes->values, sizeof(Byte) * new_capacity);
        if (bytes->values == NULL) {
            error("failure to reallocate bytes array");
        }
        bytes->capacity = new_capacity;
    }
    bytes->values[(bytes->count)++] = byte;
}

// Retrieve the byte value stored at a particular index
// If index is out of range, append zero bytes as needed
size_t bytes_get(Bytes * bytes, size_t index)
{
    while (bytes->count <= index) {
        bytes_append(bytes, 0);
    }
    return bytes->values[index];
}

// Set the byte value sttored at a particular index
// If index is out of range, append zero bytes as needed
void bytes_set(Bytes * bytes, size_t index, Byte byte)
{
    while (bytes->count <= index) {
        bytes_append(bytes, 0);
    }
    bytes->values[index] = byte;
}

// Struct representing the infinite array of Brainf*ck memory cells
// (both positive and negative addresses are supported)
typedef struct {
    Bytes positive;
    Bytes negative;
} Memory;

// Allocate initial heap memory for the memory struct
void memory_initialize(Memory * memory)
{
    bytes_initialize(&memory->positive);
    bytes_initialize(&memory->negative);
}

// Deallocate the heap memory being used by the memory struct
void memory_destroy(Memory * memory)
{
    bytes_destroy(&memory->positive);
    bytes_destroy(&memory->negative);
}

// Retrieve the value of the byte in the current memory cell
Byte memory_get(Memory * memory, Address address)
{
    size_t index;
    if (address >= 0) {
        index = address;
        return bytes_get(&memory->positive, index);
    } else {  // The address is negative
        index = -(address + 1);
        return bytes_get(&memory->negative, index);
    }
}

// Set the byte in the current memory cell to a particular value
void memory_set(Memory * memory, Address address, Byte value)
{
    size_t index;
    if (address >= 0) {
        index = address;
        bytes_set(&memory->positive, index, value);
    } else {  // The address is negative
        index = -(address + 1);
        bytes_set(&memory->negative, index, value);
    }
}

// Increment the byte in the current memory cell by one
void memory_increment(Memory * memory, Address address)
{
    Byte value = memory_get(memory, address);
    memory_set(memory, address, ++value);
}

// Decrement the byte in the current memory cell by one
void memory_decrement(Memory * memory, Address address)
{
    Byte value = memory_get(memory, address);
    memory_set(memory, address, --value);
}

// Write one byte from the current memory cell to stdout
void memory_to_stdout(Memory * memory, Address address)
{
    int c = memory_get(memory, address);
    putchar(c);
}

// Load one byte into the current memory cell from stdin
void memory_from_stdin(Memory * memory, Address address)
{
    int c = getchar();
    if (c == EOF) {
        memory_set(memory, address, 0);
    } else {
        memory_set(memory, address, c);
    }
}

/* The following macros correspond directly to each of the eight Brainf*ck
 * instructions. Given the local variables referenced here, these macros only
 * make sense within the context of the main() function. Note: most of them
 * (but not all of them) *intentionally* include semicolons.
 */
#define MOVE_RIGHT ++address;
#define MOVE_LEFT --address;
#define INCREMENT memory_increment(&memory, address);
#define DECREMENT memory_decrement(&memory, address);
#define PUT_CHAR memory_to_stdout(&memory, address);
#define GET_CHAR memory_from_stdin(&memory, address);
#define LOOP_START while (memory_get(&memory, address) != 0) {
#define LOOP_END }

// Skeleton main() function to be filled with the appropriate instructions
int main()
{
    Address address;
    Memory memory;

    address = 0;
    memory_initialize(&memory);

    /* INSERT INSTRUCTIONS HERE */

    memory_destroy(&memory);

    return EXIT_SUCCESS;
}
