#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

void error(const char * message)
{
    printf("ERROR: %s\n", message);
    exit(EXIT_FAILURE);
}

typedef bool Bit;
#define ZERO ((Bit)false)
#define ONE ((Bit)true)

typedef int64_t Address;

typedef struct {
    Bit * values;
    size_t capacity;
    size_t count;
} Bits;

#define BITS_INITIAL_CAPACITY 1
#define BITS_REALLOC_FACTOR 2

void bits_initialize(Bits * bits)
{
    bits->values = malloc(sizeof(Bit) * BITS_INITIAL_CAPACITY);
    if (bits->values == NULL)
    {
        error("failure to allocate bits array");
    }
    bits->capacity = BITS_INITIAL_CAPACITY;
    bits->count = 0;
}

void bits_destroy(Bits * bits)
{
    free(bits->values);
    bits->values = NULL;
    bits->capacity = 0;
    bits->count = 0;
}

void bits_append(Bits * bits, Bit bit)
{
    if (bits->capacity <= bits->count)
    {
        bits->values = realloc(
            bits->values, bits->capacity * BITS_REALLOC_FACTOR
        );
        if (bits->values == NULL)
        {
            error("failure to reallocate bits array");
        }
        bits->capacity *= BITS_REALLOC_FACTOR;
    }

    bits->values[bits->count] = bit;
    bits->count += 1;
}

Bit bits_get(Bits * bits, size_t index)
{
    while (bits->count <= index)
    {
        bits_append(bits, ZERO);
    }
    return bits->values[index];
}

void bits_set(Bits * bits, size_t index, Bit bit)
{
    while (bits->count <= index)
    {
        bits_append(bits, ZERO);
    }
    bits->values[index] = bit;
}

typedef struct {
    Bits positive;
    Bits negative;
} Memory;

void memory_initialize(Memory * memory)
{
    bits_initialize(&(memory->positive));
    bits_initialize(&(memory->negative));
}

void memory_destroy(Memory * memory)
{
    bits_destroy(&(memory->positive));
    bits_destroy(&(memory->negative));
}

Bit memory_get(Memory * memory, Address address)
{
    size_t index;

    if (address >= 0)
    {
        index = address;
        return bits_get(&(memory->positive), index);
    }
    else  // The address is negative
    {
        index = -(address + 1);
        return bits_get(&(memory->negative), index);
    }
}

void memory_set(Memory * memory, Address address, Bit value)
{
    size_t index;

    if (address >= 0)
    {
        index = address;
        bits_set(&(memory->positive), index, value);
    }
    else  // The address is negative
    {
        index = -(address + 1);
        bits_set(&(memory->negative), index, value);
    }
}

void memory_increment(Memory * memory, Address address)
{
    Bit new_value = !memory_get(memory, address);
    memory_set(memory, address, new_value);
}

void output_bit(Memory * memory, Address address)
{
    static uint8_t buffer = 0;
    static size_t bits_buffered = 0;

    Bit bit = memory_get(memory, address);
    buffer = (buffer << 1) | ((uint8_t) bit);
    bits_buffered += 1;

    if (bits_buffered == 8)
    {
        char c = buffer;
        putchar(c);
        buffer = 0;
        bits_buffered = 0;
    }
}

void input_bit(Memory * memory, Address address)
{
    static uint8_t buffer = 0;
    static size_t bits_buffered = 0;

    if (bits_buffered == 0)
    {
        char c = getchar();
        if (c == EOF)
        {
            c = '\0';
        }
        buffer = (uint8_t) getchar();
        bits_buffered = 8;
    }

    bit = (buffer & (1 << (bits_buffered - 1))) != 0;
    memory_set(memory, address, bit);
}

#define MOVE_RIGHT address += 1;
#define MOVE_LEFT address -= 1;
#define INCREMENT memory_increment(&memory, address);
#define PUT_BIT output_bit(&memory, address);
#define GET_CHAR input_bit(&memory, address);
#define LOOP_START while (memory_get(&memory, address) != ZERO) {
#define LOOP_END }

int main()
{
    Address address = 0;
    (void) address;  // Ignore "unused" variable
    Memory memory;
    memory_initialize(&memory);

    /* INSERT INSTRUCTIONS HERE */

    memory_destroy(&memory);
    return EXIT_SUCCESS;
}
