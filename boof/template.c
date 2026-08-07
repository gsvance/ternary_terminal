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

// Print an error message and terminate the program
void error(const char * message)
{
    fputs("ERROR: ", stderr);
    fputs(message, stderr);
    fputs("\n", stderr);
    exit(EXIT_FAILURE);
}

// Underlying types to use for the boolf*ck memory cells and address pointer
typedef uint8_t Bit;  // Eight bits but we're only using one :(
typedef int64_t Address;

// Dynamic array of bit values that can be reallocated as needed
typedef struct {
    Bit * values;
    size_t capacity;
    size_t count;
} Bits;

// How large should the dynamic bits arrays be when first allocated?
#define BITS_INITIAL_CAPACITY 2

// By how much should the dynamic bits arrays expand when they reallocate?
#define BITS_REALLOC_FACTOR 2

// Allocate initial heap memory for the dynamic bits array
void bits_initialize(Bits * bits)
{
    bits->values = malloc(sizeof(Bit) * BITS_INITIAL_CAPACITY);
    if (bits->values == NULL) {
        error("failure to allocate bits array");
    }
    bits->capacity = BITS_INITIAL_CAPACITY;
    bits->count = 0;
}

// Deallocate the heap memory being used by the dynamic bits array
void bits_destroy(Bits * bits)
{
    free(bits->values);
    bits->values = NULL;
    bits->count = bits->capacity = 0;
}

// Append a new bit value to the end of the dynamic array of bits
void bits_append(Bits * bits, Bit bit)
{
    if (bits->capacity <= bits->count) {
        size_t new_capacity = bits->capacity * BITS_REALLOC_FACTOR;
        bits->values = realloc(bits->values, sizeof(Bit) * new_capacity);
        if (bits->values == NULL) {
            error("failure to reallocate bits array");
        }
        bits->capacity = new_capacity;
    }

    bits->values[(bits->count)++] = bit;
}

// Retrieve the bit value stored at a particular index
// If index is out of range, append zero bits as needed
Bit bits_get(Bits * bits, size_t index)
{
    while (bits->count <= index) {
        bits_append(bits, 0);
    }
    return bits->values[index];
}

// Set the bit value stored at a particular index
// If index is out of range, append zero bits as needed
void bits_set(Bits * bits, size_t index, Bit bit)
{
    while (bits->count <= index) {
        bits_append(bits, 0);
    }
    bits->values[index] = bit;
}

// Struct representing the infinite array of boolf*ck memory cells
// (both positive and negative addresses are supported)
typedef struct {
    Bits positive;
    Bits negative;
} Memory;

// Allocate initial heap memory for the memory struct
void memory_initialize(Memory * memory)
{
    bits_initialize(&memory->positive);
    bits_initialize(&memory->negative);
}

// Deallocate the heap memory being used by the memory struct
void memory_destroy(Memory * memory)
{
    bits_destroy(&memory->positive);
    bits_destroy(&memory->negative);
}

// Retrieve the value of the bit in the current memory cell
Bit memory_get(Memory * memory, Address address)
{
    size_t index;
    if (address >= 0) {
        index = address;
        return bits_get(&memory->positive, index);
    } else {  // The address is negative
        index = -(address + 1);
        return bits_get(&memory->negative, index);
    }
}

// Set the bit in the current memory cell to a particular value
void memory_set(Memory * memory, Address address, Bit value)
{
    size_t index;
    if (address >= 0) {
        index = address;
        bits_set(&memory->positive, index, value);
    } else {  // The address is negative
        index = -(address + 1);
        bits_set(&memory->negative, index, value);
    }
}

// Flip the bit in the current memory cell from 0 to 1 or from 1 to 0
void memory_flip(Memory * memory, Address address)
{
    Bit value = memory_get(memory, address);
    memory_set(memory, address, (value == 0) ? 1 : 0);
}

// Write one bit from the current memory cell to stdout
void output_bit(Memory * memory, Address address)
{
    static uint8_t buffer = 0;
    static size_t bits_buffered = 0;

    Bit bit = memory_get(memory, address);
    buffer |= ((bit == 0) ? 0 : 1) << (bits_buffered++);

    if (bits_buffered == 8) {
        int c = buffer;
        putchar(c);
        buffer = 0;
        bits_buffered = 0;
    }
}

// Load one bit into the current memory cell from stdin
void input_bit(Memory * memory, Address address)
{
    static uint8_t buffer = 0;
    static size_t bits_buffered = 0;

    if (bits_buffered == 0) {
        int c = getchar();
        buffer = (c == EOF) ? 0 : c;
        bits_buffered = 8;
    }

    unsigned mask = 0x100 >> (bits_buffered--);
    Bit bit = ((buffer & mask) == 0) ? 0 : 1;
    memory_set(memory, address, bit);
}

/* The following macros correspond directly to each of the seven boolf*ck
 * instructions. Given the local variables referenced here, these macros only
 * make sense within the context of the main() function. Note: most of them
 * (but not all of them) *intentionally* include semicolons.
 */
#define MOVE_RIGHT ++address;
#define MOVE_LEFT --address;
#define FLIP_BIT memory_flip(&memory, address);
#define PUT_BIT output_bit(&memory, address);
#define GET_BIT input_bit(&memory, address);
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

    // Flush the output buffer to stdout by setting the current memory cell to
    // zero and then outputting seven more zero bits
    LOOP_START FLIP_BIT LOOP_END
    PUT_BIT PUT_BIT PUT_BIT PUT_BIT PUT_BIT PUT_BIT PUT_BIT

    memory_destroy(&memory);
    (void) address;  // Silence warnings about unused variable

    return EXIT_SUCCESS;
}
