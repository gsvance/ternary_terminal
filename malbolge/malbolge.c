/*
 *
*/

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

// The number of different values a trit can hold
#define TRIT_RANGE 3

// The number of trits that make up one word
#define WORD_TRITS 10

// The number of different values one word can hold
#define WORD_RANGE 59049  // pow(TRIT_RANGE, WORD_TRITS)

// Helper macro for switch-case that implements the crazy operation
#define BI_TRIT_CODE(t1, t2) ((unsigned) (t1 * TRIT_RANGE + t2))

// Modulus used for decoding Malbolge instructions
#define DECODING_MODULUS 94

// Minimum and maximum values for non-halting instructions
#define NON_HALTING_INSTRUCTION_MIN 33
#define NON_HALTING_INSTRUCTION_MAX 126

//
const char enciphering_table[DECODING_MODULUS + 1] = (
    "9m<.TVac`uY*MK'X~xDl}REokN:#?G\"i@5z]&gqtyfr$(we"
    "4{WP)H-Zn,[%\3dL+Q;>U!pJS72FhOA1CB6v^=I_0/8|jsb"
);

// A single trit in the Malbolge virtual machine
typedef enum : uint8_t {
    TRIT_ZERO = 0,
    TRIT_ONE = 1,
    TRIT_TWO = 2,
} Trit;

// A single machine word in the Malbolge virutal machine
typedef struct {
    Trit trits[WORD_TRITS];
} Word;

// Values corresponding to the 8 valid Malbolge instructions
typedef enum : unsigned {
    INSTRUCTION_JUMP = 4,
    INSTRUCTION_OUTPUT = 5,
    INSTRUCTION_INPUT = 23,
    INSTRUCTION_ROTATE = 39,
    INSTRUCTION_COPY = 40,
    INSTRUCTION_CRAZY = 62,
    INSTRUCTION_NOOP = 68,
    INSTRUCTION_HALT = 81,
} Instruction;

// Print an error message to stderr and then stop execution
noreturn void error(const char * message)
{
    fputs("ERROR: ", stderr);
    fputs(message, stderr);
    fputs("\n", stderr);

    exit(EXIT_FAILURE);
}

// Safely cast an integer value to a trit
Trit as_trit(unsigned value)
{
    if (value >= TRIT_RANGE) {
        error("new trit value out of range");
    }
    return value;
}

// Increment the value of a trit, wrapping to 0 when it maxes out
Trit next_trit(Trit trit)
{
    unsigned value = trit;
    value = (value + 1u) % (unsigned) TRIT_RANGE;
    return as_trit(value);
}

// Convert an integer value to a word
Word as_word(unsigned value)
{
    Word word;

    for (int i = WORD_TRITS - 1; i >= 0; i--) {
        unsigned remainder = value % (unsigned) TRIT_RANGE;
        word.trits[i] = as_trit(remainder);
        value /= (unsigned) TRIT_RANGE;
    }

    if (value > 0) {
        error("new word value out of range");
    }
    return word;
}

// Convert a word to an integer value
unsigned as_int(Word word)
{
    unsigned result = 0;

    for (int i = 0; i < WORD_TRITS; i++) {
        result *= (unsigned) TRIT_RANGE;
        result += (unsigned) word.trits[i];
    }

    return result;
}

// Move each trit one place to the right, wrapping the rightmost to the left
Word rotate_right(Word x)
{
    Trit rightmost = x.trits[WORD_TRITS - 1];

    Trit * source = &(x.trits[0]);
    size_t n_bytes = sizeof(*source) * (WORD_TRITS - 1);
    Trit * destination = &(x.trits[1]);
    memmove(destination, source, n_bytes);

    x.trits[0] = rightmost;
    return x;
}

// Implement Malbolge's nonsensical crazy operation for two words
Word crazy_operation(Word x, Word y)
{
    Word crazy;

    for (int i = 0; i < WORD_TRITS; i++) {
        unsigned bi_trit_code = BI_TRIT_CODE(x.trits[i], y.trits[i]);
        switch (bi_trit_code) {

            case BI_TRIT_CODE(TRIT_ONE, TRIT_ZERO):
            case BI_TRIT_CODE(TRIT_ONE, TRIT_ONE):
            case BI_TRIT_CODE(TRIT_TWO, TRIT_ZERO):
                crazy.trits[i] = TRIT_ZERO;
                break;

            case BI_TRIT_CODE(TRIT_ZERO, TRIT_ZERO):
            case BI_TRIT_CODE(TRIT_ZERO, TRIT_ONE):
            case BI_TRIT_CODE(TRIT_TWO, TRIT_TWO):
                crazy.trits[i] = TRIT_ONE;
                break;

            case BI_TRIT_CODE(TRIT_ZERO, TRIT_TWO):
            case BI_TRIT_CODE(TRIT_ONE, TRIT_TWO):
            case BI_TRIT_CODE(TRIT_TWO, TRIT_ONE):
                crazy.trits[i] = TRIT_TWO;
                break;

            default:
                error("unreachable value of bi-trit code");
        }
    }

    return crazy;
}

// Increment a word to the next value, wrapping to 0 when it maxes out
Word next_word(Word x)
{
    x.trits[WORD_TRITS - 1] = next_trit(x.trits[WORD_TRITS - 1]);

    for (int i = WORD_TRITS - 1; i >= 1; i--) {
        if (x.trits[i] != TRIT_ZERO) {
            break;
        }
        x.trits[i - 1] = next_trit(x.trits[i - 1]);
    }

    return x;
}

// Decode the word at index c as a Malbolge instruction
unsigned decode_instruction(const Word * memory, Word c)
{
    unsigned c_as_int = as_int(c);
    Word word_at_c = memory[c_as_int];
    return (as_int(word_at_c) + c_as_int) % (unsigned) DECODING_MODULUS;
}

// Encipher an instruction in memory (used right after it is executed)
void encipher(Word * memory, Word c)
{
    unsigned instruction = decode_instruction(memory, c);
    memory[as_int(c)] = as_word(enciphering_table[instruction]);
}

// Load a Malbolge program into the machine's memory from an input file
void load_program(Word * memory, const char * file_name)
{
    FILE * infile = fopen(file_name, "r");
    if (infile == NULL) {
        error("failed to open input file");
    }

    size_t index = 0;
    int character;
    while ((character = getc(infile)) != EOF)
    {
        if (isspace(character)) {
            continue;
        }
        if (index == WORD_RANGE) {
            error("input file is too long");
        }

        memory[index] = as_word(character);
        unsigned instruction = decode_instruction(memory, as_word(index));

        switch (instruction) {
            case INSTRUCTION_JUMP:
            case INSTRUCTION_OUTPUT:
            case INSTRUCTION_INPUT:
            case INSTRUCTION_ROTATE:
            case INSTRUCTION_COPY:
            case INSTRUCTION_CRAZY:
            case INSTRUCTION_NOOP:
            case INSTRUCTION_HALT:
                break;
            default:
                error("illegal character in input file");
        }

        index += 1;
    }

    fclose(infile);
    if (index < 2) {
        error("input file is too short");
    }

    for (; index < WORD_RANGE; index++) {
        memory[index] = crazy_operation(memory[index - 2], memory[index - 1]);
    }
}

// Execute the Malbolge program in memory
void execute_program(Word * memory)
{
    // Registers
    Word a = as_word(0);  // Accumulator
    Word c = as_word(0);  // Code pointer
    Word d = as_word(0);  // Data pointer

    while (true) {
        unsigned int_at_c = as_int(memory[as_int(c)]);
        if (int_at_c < NON_HALTING_INSTRUCTION_MIN) {
            return;
        }
        if (int_at_c > NON_HALTING_INSTRUCTION_MAX) {
            return;
        }

        switch (decode_instruction(memory, c)) {
            case INSTRUCTION_JUMP:
                c = memory[as_int(d)];
                break;
            case INSTRUCTION_OUTPUT:
                char character = as_int(a) % 256u;
                if (character == 10) {
                    putchar('\n');
                } else {
                    putchar(character);
                }
                break;
            case INSTRUCTION_INPUT: {
                int character = getchar();
                if (character == EOF) {
                    a = as_word(WORD_RANGE - 1);
                } else if (character == '\n') {
                    a = as_word(10);
                } else {
                    a = as_word((unsigned) character);
                }
                break;
            }
            case INSTRUCTION_ROTATE: {
                unsigned d_as_int = as_int(d);
                a = (memory[d_as_int] = rotate_right(memory[d_as_int]));
                break;
            }
            case INSTRUCTION_COPY:
                d = memory[as_int(d)];
                break;
            case INSTRUCTION_CRAZY: {
                unsigned d_as_int = as_int(d);
                a = (memory[d_as_int] = crazy_operation(a, memory[d_as_int]));
                break;
            }
            case INSTRUCTION_NOOP:
                break;
            case INSTRUCTION_HALT:
                return;
            default:
                break;
        }

        encipher(memory, c);
        c = next_word(c);
        d = next_word(d);
    }
}

// Read input file name from command line, set up Malbolge, and then run it
int main(int argc, char ** argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    Word * memory = malloc(sizeof(Word) * WORD_RANGE);
    if (memory == NULL) {
        error("interpreter unable to allocate memory");
    }

    load_program(memory, argv[1]);
    execute_program(memory);

    free(memory);
    return EXIT_SUCCESS;
}
