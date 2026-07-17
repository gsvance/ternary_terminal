#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void error(const char * message)
{
    printf("ERROR: %s\n", message);
    exit(EXIT_FAILURE);
}

typedef uint8_t Byte;
typedef int64_t Address;

typedef struct {
    Byte * values;
    size_t capacity;
    size_t count;
} Bytes;

#define BYTES_INITIAL_CAPACITY 1
#define BYTES_REALLOC_FACTOR 2

void bytes_initialize(Bytes * bytes)
{
    bytes->values = malloc(sizeof(Byte) * BYTES_INITIAL_CAPACITY);
    if (bytes->values == NULL)
    {
        error("failure to allocate bytes array");
    }
    bytes->capacity = BYTES_INITIAL_CAPACITY;
    bytes->count = 0;
}

void bytes_destroy(Bytes * bytes)
{
    free(bytes->values);
    bytes->values = NULL;
    bytes->capacity = 0;
    bytes->count = 0;
}

void bytes_append(Bytes * bytes, Byte byte)
{
    if (bytes->capacity <= bytes->count)
    {
        bytes->values = realloc(
            bytes->values, bytes->capacity * BYTES_REALLOC_FACTOR
        );
        if (bytes->values == NULL)
        {
            error("failure to reallocate bytes array");
        }
        bytes->capacity *= BYTES_REALLOC_FACTOR;
    }

    bytes->values[bytes->count] = byte;
    bytes->count += 1;
}

size_t bytes_get(Bytes * bytes, size_t index)
{
    while (bytes->count <= index)
    {
        bytes_append(bytes, 0);
    }
    return bytes->values[index];
}

void bytes_set(Bytes * bytes, size_t index, Byte byte)
{
    while (bytes->count <= index)
    {
        bytes_append(bytes, 0);
    }
    bytes->values[index] = byte;
}

typedef struct {
    Bytes positive;
    Bytes negative;
} Memory;

void memory_initialize(Memory * memory)
{
    bytes_initialize(&(memory->positive));
    bytes_initialize(&(memory->negative));
}

void memory_destroy(Memory * memory)
{
    bytes_destroy(&(memory->positive));
    bytes_destroy(&(memory->negative));
}

Byte memory_get(Memory * memory, Address address)
{
    size_t index;

    if (address >= 0)
    {
        index = address;
        return bytes_get(&(memory->positive), index);
    }
    else  // The address is negative
    {
        index = -(address + 1);
        return bytes_get(&(memory->negative), index);
    }
}

void memory_set(Memory * memory, Address address, Byte value)
{
    size_t index;

    if (address >= 0)
    {
        index = address;
        bytes_set(&(memory->positive), index, value);
    }
    else  // The address is negative
    {
        index = -(address + 1);
        bytes_set(&(memory->negative), index, value);
    }
}

void memory_increment(Memory * memory, Address address)
{
    Byte new_value = memory_get(memory, address) + 1;
    memory_set(memory, address, new_value);
}

void memory_decrement(Memory * memory, Address address)
{
    Byte new_value = memory_get(memory, address) - 1;
    memory_set(memory, address, new_value);
}

#define MOVE_RIGHT address += 1;
#define MOVE_LEFT address -= 1;
#define INCREMENT memory_increment(&memory, address);
#define DECREMENT memory_decrement(&memory, address);
#define PUT_CHAR putchar(memory_get(&memory, address));
#define GET_CHAR memory_set(&memory, address, getchar());
#define LOOP_START while (memory_get(&memory, address) != 0) {
#define LOOP_END }

int main()
{
    Address address = 0;
    Memory memory;
    memory_initialize(&memory);

    /* INSERT INSTRUCTIONS HERE */

    memory_destroy(&memory);
    return EXIT_SUCCESS;
}
