#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void error(const char * message)
{
    eprintf("ERROR: %s\n", message);
    exit(EXIT_FAILURE);
}

#define WORD_SIZE 10
#define POW_3_10 (3*3*3*3*3 * 3*3*3*3*3)

typedef enum : uint8_t {ZERO = 0, ONE = 1, TWO = 2} Trit;

typedef struct {
    Trit trits[WORD_SIZE];
} Word;

Trit trit_new(int value)
{
    switch (value)
    {
        case ZERO:
            return ZERO;
        case ONE:
            return ONE;
        case TWO:
            return TWO;
        default:
            error("invalid trit value");
    }
}

Word word_new(int value)
{
    if (value < 0)
    {
        error("negative word value");
    }

    Word word;
    
    for (size_t i = 0; i < WORD_SIZE; ++i)
    {
        word.trits[(WORD_SIZE - 1) - i] = trit_new(value % 3);
        value /= 3;
    }

    if (value != 0)
    {
        error("overflow word value");
    }
    
    return word;
}

size_t word_as_index(Word word)
{
    size_t index = 0;

    for (size_t i = 0; i < WORD_SIZE; ++i)
    {
        index = (index * 3) + ((size_t) word.trits[i]);
    }

    return index;
}

Word crazy(Word x, Word y)
{
    Word result;

    for (size_t i = 0; i < WORD_SIZE; ++i)
    {
        int code = 3 * ((int) x.trits[i]) + ((int) y.trits[i]);
        switch (code)
        {
            case (3*ZERO + ZERO):
                result.trits[i] = new_trit(ONE);
                break;
            case (3*ZERO + ONE):
                result.trits[i] = new_trit(ZERO);
                break;
            case (3*ZERO + TWO):
                result.trit[i] = new_trit(ZERO);
                break;
            case (3*ONE + ZERO):
                result.trit[i] = new_trit(ONE);
                break;
            case (3*ONE + ONE):
                result.trit[i] = new_trit(ZERO);
                break;
            case (3*ONE + TWO):
                result.trit[i] = new_trit(TWO);
                break;
            case (3*TWO + ZERO):
                result.trit[i] = new_trit(TWO);
                break;
            case (3*TWO + ONE):
                result.trit[i] = new_trit(TWO);
                break;
            case (3*TWO + TWO):
                result.trit[i] = new_trit(ONE);
                break;
            default:
                error("invalid crazy code");
        }
    }

    return result;
}

void load_program(Word * memory, const char * file_name)
{
    FILE * f = fopen(file_name, "r");
    if (f == NULL)
    {
        error("failed to open input file");
    }

    size_t index = 0;
    char c;
    while ((c = getc(f)) != EOF)
    {
        if (isspace(c))
        {
            continue;
        }
        // finish this
    }
    
    fclose(f);
}

void execute_program(Word * memory)
{
    // implement this
}

int main(int argc, char ** argv)
{
    if (argc != 2)
    {
        eprintf("Usage: %s file_name\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    Word * memory = malloc(sizeof(Word) * POW_3_10);
    if (memory == NULL)
    {
        eprintf("Interpreter was not able to allocate memory.\n");
        return EXIT_FAILURE;
    }
    
    load_program(memory, argv[1]);
    execute_program(memory);

    free(memory);

    return EXIT_SUCCESS;
}
