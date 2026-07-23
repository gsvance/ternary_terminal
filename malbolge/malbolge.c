#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

noreturn void error(const char * message)
{
    fprintf(stderr, "ERROR: %s\n", message);
    exit(EXIT_FAILURE);
}

#define WORD_SIZE 10u

// Number of possible values one word can hold == pow(3, WORD_SIZE)
#define WORD_RANGE (3u*3u * 3u*3u * 3u*3u * 3u*3u * 3u*3u)

typedef enum : uint8_t {ZERO = 0u, ONE = 1u, TWO = 2u} Trit;

typedef struct {
    Trit trits[WORD_SIZE];
} Word;

Word as_word(unsigned value)
{
    Word word;

    for (size_t i = 0u; i < WORD_SIZE; ++i)
    {
        size_t i_from_right = (WORD_SIZE - 1u) - i;
        word.trits[i_from_right] = value % 3u;
        value /= 3u;
    }

    if (value > 0u)
    {
        error("overflow word range");
    }

    return word;
}

unsigned as_int(Word word)
{
    unsigned result = 0u;

    for (size_t i = 0u; i < WORD_SIZE; ++i)
    {
        result = (result * 3u) + (unsigned) word.trits[i];
    }

    return result;
}

Word rotate_right(Word x)
{
    Trit rightmost = x.trits[WORD_SIZE - 1u];
    memmove(&(x.trits[1u]), &(x.trits[0u]), WORD_SIZE - 1u);
    x.trits[0u] = rightmost;
    return x;
}

#define TWO_TRIT_CODE(t1, t2) (3u * (unsigned) t1 + (unsigned) t2)

Word crazy_operation(Word x, Word y)
{
    Word result;

    for (size_t i = 0u; i < WORD_SIZE; ++i)
    {
        switch (TWO_TRIT_CODE(x.trits[i], y.trits[i]))
        {
            case TWO_TRIT_CODE(ONE, ZERO):
            case TWO_TRIT_CODE(ONE, ONE):
            case TWO_TRIT_CODE(TWO, ZERO):
                result.trits[i] = ZERO;
                break;
            case TWO_TRIT_CODE(ZERO, ZERO):
            case TWO_TRIT_CODE(ZERO, ONE):
            case TWO_TRIT_CODE(TWO, TWO):
                result.trits[i] = ONE;
                break;
            case TWO_TRIT_CODE(ZERO, TWO):
            case TWO_TRIT_CODE(ONE, TWO):
            case TWO_TRIT_CODE(TWO, ONE):
                result.trits[i] = TWO;
                break;
            default:
                error("unreachable two-trit code");
                break;
        }
    }

    return result;
}

Word increment(Word x)
{
    x.trits[WORD_SIZE - 1u] = ((unsigned) x.trits[WORD_SIZE - 1u] + 1u) % 3u;

    for (size_t i = 1; i < WORD_SIZE; ++i)
    {
        size_t i_from_right = (WORD_SIZE - 1u) - i;
        if (x.trits[i_from_right + 1u] != ZERO)
        {
            break;
        }
        x.trits[i_from_right] = ((unsigned) x.trits[i_from_right] + 1u) % 3u;
    }

    return x;
}

#define INSTRUCTION_MODULUS 94u

#define JUMP 4u
#define OUTPUT 5u
#define INPUT 23u
#define ROTATE 39u
#define COPY 40u
#define CRAZY 62u
#define NOOP 68u
#define HALT 81u

const char enciphering_table[INSTRUCTION_MODULUS + 1u] = (
    "9m<.TVac`uY*MK'X~xDl}REokN:#?G\"i@5z]&gqtyfr$(we"
    "4{WP)H-Zn,[%\3dL+Q;>U!pJS72FhOA1CB6v^=I_0/8|jsb"
);

unsigned decode_instruction(const Word * memory, Word c)
{
    unsigned c_as_int = as_int(c);
    Word word_at_c = memory[c_as_int];
    return (as_int(word_at_c) + c_as_int) % INSTRUCTION_MODULUS;
}

void encipher(Word * memory, Word c)
{
    unsigned instruction = decode_instruction(memory, c);
    memory[as_int(c)] = as_word(enciphering_table[instruction]);
}

void load_program(Word * memory, const char * file_name)
{
    FILE * f = fopen(file_name, "r");
    if (f == NULL)
    {
        error("failed to open input file");
    }

    size_t index = 0u;
    char character;
    while ((character = getc(f)) != EOF)
    {
        if (isspace(character))
        {
            continue;
        }

        if (index == WORD_RANGE)
        {
            error("input file is too long");
        }

        memory[index] = as_word(character);
        unsigned instruction = decode_instruction(memory, as_word(index));

        switch (instruction)
        {
            case JUMP:
            case OUTPUT:
            case INPUT:
            case ROTATE:
            case COPY:
            case CRAZY:
            case NOOP:
            case HALT:
                break;
            default:
                error("illegal character in input file");
                break;
        }

        ++index;
    }

    fclose(f);

    if (index < 2u)
    {
        error("input file is too short");
    }

    for (; index < WORD_RANGE; ++index)
    {
        memory[index] = crazy_operation(
            memory[index - 2u], memory[index - 1u]
        );
    }
}

void execute_program(Word * memory)
{
    // Registers
    Word a = as_word(0);  // Accumulator
    Word c = as_word(0);  // Code pointer
    Word d = as_word(0);  // Data pointer

    while (true)
    {
        unsigned int_at_c = as_int(memory[as_int(c)]);
        if (int_at_c < 33 || int_at_c > 126)
        {
            return;
        }

        switch (decode_instruction(memory, c))
        {
            case JUMP:
                c = memory[as_int(d)];
                break;
            case OUTPUT:
                char character = as_int(a) % 256u;
                if (character == 10)
                {
                    putchar('\n');
                }
                else
                {
                    putchar(character);
                }
                break;
            case INPUT:
            {
                int character = getchar();
                if (character == EOF)
                {
                    a = as_word(WORD_RANGE - 1u);
                }
                else if (character == '\n')
                {
                    a = as_word(10u);
                }
                else
                {
                    a = as_word((uint8_t) character);
                }
                break;
            }
            case ROTATE:
            {
                unsigned d_as_int = as_int(d);
                a = (memory[d_as_int] = rotate_right(memory[d_as_int]));
                break;
            }
            case COPY:
                d = memory[as_int(d)];
                break;
            case CRAZY:
            {
                unsigned d_as_int = as_int(d);
                a = (memory[d_as_int] = crazy_operation(a, memory[d_as_int]));
                break;
            }
            case NOOP:
                break;
            case HALT:
                return;
            default:
                break;
        }

        encipher(memory, c);
        c = increment(c);
        d = increment(d);
    }
}

int main(int argc, char ** argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    Word * memory = malloc(sizeof(Word) * WORD_RANGE);
    if (memory == NULL)
    {
        fprintf(stderr, "Interpreter was not able to allocate memory.\n");
        return EXIT_FAILURE;
    }

    load_program(memory, argv[1]);
    execute_program(memory);

    free(memory);

    return EXIT_SUCCESS;
}
