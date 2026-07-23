#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdnoreturn.h>
#include <ctype.h>

noreturn void error(const char * message)
{
    fprintf(stderr, "ERROR: %s\n", message);
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

#define trit_as_int(trit) ((int) (trit))

Word word_new(int value)
{
    if (value < 0)
    {
        error("negative word value");
    }

    Word word;
    
    for (int i = WORD_SIZE - 1; i >= 0; i--)
    {
        word.trits[i] = trit_new(value % 3);
        value /= 3;
    }

    if (value != 0)
    {
        error("overflow word value");
    }
    
    return word;
}

int word_as_int(Word word)
{
    int result = 0;

    for (int i = 0; i < WORD_SIZE; ++i)
    {
        result = (result * 3) + trit_as_int(word.trits[i]);
    }

    return result;
}

Word rotate_right(Word x)
{
    Trit rightmost = x.trits[WORD_SIZE - 1];

    for (int i = WORD_SIZE - 1; i > 0; i--)
    {
        x.trits[i] = x.trits[i - 1];
    }

    x.trits[0] = rightmost;
    return x;
}

#define TWO_TRIT_CODE(trit_1, trit_2) (3 * trit_as_int(trit_1) + trit_as_int(trit_2))

Word crazy_operation(Word x, Word y)
{
    Word result;

    for (int i = 0; i < WORD_SIZE; ++i)
    {
        switch (TWO_TRIT_CODE(x.trits[i], y.trits[i]))
        {
            case TWO_TRIT_CODE(ONE, ZERO):
            case TWO_TRIT_CODE(ONE, ONE):
            case TWO_TRIT_CODE(TWO, ZERO):
                result.trits[i] = trit_new(ZERO);
                break;
            case TWO_TRIT_CODE(ZERO, ZERO):
            case TWO_TRIT_CODE(ZERO, ONE):
            case TWO_TRIT_CODE(TWO, TWO):
                result.trits[i] = trit_new(ONE);
                break;
            case TWO_TRIT_CODE(ZERO, TWO):
            case TWO_TRIT_CODE(ONE, TWO):
            case TWO_TRIT_CODE(TWO, ONE):
                result.trits[i] = trit_new(TWO);
                break;
            default:
                error("invalid trits in crazy operation");
        }
    }

    return result;
}

void encipher(Word * memory, Word c)
{
    int result = word_as_int(memory[word_as_int(c)]) % 94;

    switch (result)
    {
        case 0:
            memory[word_as_int(c)] = word_new(57);
            break;
        case 1:
            memory[word_as_int(c)] = word_new(109);
            break;
        case 2:
            memory[word_as_int(c)] = word_new(60);
            break;
        case 3:
            memory[word_as_int(c)] = word_new(46);
            break;
        case 4:
            memory[word_as_int(c)] = word_new(84);
            break;
        case 5:
            memory[word_as_int(c)] = word_new(86);
            break;
        case 6:
            memory[word_as_int(c)] = word_new(97);
            break;
        case 7:
            memory[word_as_int(c)] = word_new(99);
            break;
        case 8:
            memory[word_as_int(c)] = word_new(96);
            break;
        case 9:
            memory[word_as_int(c)] = word_new(117);
            break;
        case 10:
            memory[word_as_int(c)] = word_new(89);
            break;
        case 11:
            memory[word_as_int(c)] = word_new(42);
            break;
        case 12:
            memory[word_as_int(c)] = word_new(77);
            break;
        case 13:
            memory[word_as_int(c)] = word_new(75);
            break;
        case 14:
            memory[word_as_int(c)] = word_new(39);
            break;
        case 15:
            memory[word_as_int(c)] = word_new(88);
            break;
        case 16:
            memory[word_as_int(c)] = word_new(126);
            break;
        case 17:
            memory[word_as_int(c)] = word_new(120);
            break;
        case 18:
            memory[word_as_int(c)] = word_new(68);
            break;
        case 19:
            memory[word_as_int(c)] = word_new(108);
            break;
        case 20:
            memory[word_as_int(c)] = word_new(125);
            break;
        case 21:
            memory[word_as_int(c)] = word_new(82);
            break;
        case 22:
            memory[word_as_int(c)] = word_new(69);
            break;
        case 23:
            memory[word_as_int(c)] = word_new(111);
            break;
        case 24:
            memory[word_as_int(c)] = word_new(107);
            break;
        case 25:
            memory[word_as_int(c)] = word_new(78);
            break;
        case 26:
            memory[word_as_int(c)] = word_new(58);
            break;
        case 27:
            memory[word_as_int(c)] = word_new(35);
            break;
        case 28:
            memory[word_as_int(c)] = word_new(63);
            break;
        case 29:
            memory[word_as_int(c)] = word_new(71);
            break;
        case 30:
            memory[word_as_int(c)] = word_new(34);
            break;
        case 31:
            memory[word_as_int(c)] = word_new(105);
            break;
        case 32:
            memory[word_as_int(c)] = word_new(64);
            break;
        case 33:
            memory[word_as_int(c)] = word_new(53);
            break;
        case 34:
            memory[word_as_int(c)] = word_new(122);
            break;
        case 35:
            memory[word_as_int(c)] = word_new(93);
            break;
        case 36:
            memory[word_as_int(c)] = word_new(38);
            break;
        case 37:
            memory[word_as_int(c)] = word_new(103);
            break;
        case 38:
            memory[word_as_int(c)] = word_new(113);
            break;
        case 39:
            memory[word_as_int(c)] = word_new(116);
            break;
        case 40:
            memory[word_as_int(c)] = word_new(121);
            break;
        case 41:
            memory[word_as_int(c)] = word_new(102);
            break;
        case 42:
            memory[word_as_int(c)] = word_new(114);
            break;
        case 43:
            memory[word_as_int(c)] = word_new(36);
            break;
        case 44:
            memory[word_as_int(c)] = word_new(40);
            break;
        case 45:
            memory[word_as_int(c)] = word_new(119);
            break;
        case 46:
            memory[word_as_int(c)] = word_new(101);
            break;
        case 47:
            memory[word_as_int(c)] = word_new(52);
            break;
        case 48:
            memory[word_as_int(c)] = word_new(123);
            break;
        case 49:
            memory[word_as_int(c)] = word_new(87);
            break;
        case 50:
            memory[word_as_int(c)] = word_new(80);
            break;
        case 51:
            memory[word_as_int(c)] = word_new(41);
            break;
        case 52:
            memory[word_as_int(c)] = word_new(72);
            break;
        case 53:
            memory[word_as_int(c)] = word_new(45);
            break;
        case 54:
            memory[word_as_int(c)] = word_new(90);
            break;
        case 55:
            memory[word_as_int(c)] = word_new(110);
            break;
        case 56:
            memory[word_as_int(c)] = word_new(44);
            break;
        case 57:
            memory[word_as_int(c)] = word_new(91);
            break;
        case 58:
            memory[word_as_int(c)] = word_new(37);
            break;
        case 59:
            memory[word_as_int(c)] = word_new(92);
            break;
        case 60:
            memory[word_as_int(c)] = word_new(51);
            break;
        case 61:
            memory[word_as_int(c)] = word_new(100);
            break;
        case 62:
            memory[word_as_int(c)] = word_new(76);
            break;
        case 63:
            memory[word_as_int(c)] = word_new(43);
            break;
        case 64:
            memory[word_as_int(c)] = word_new(81);
            break;
        case 65:
            memory[word_as_int(c)] = word_new(59);
            break;
        case 66:
            memory[word_as_int(c)] = word_new(62);
            break;
        case 67:
            memory[word_as_int(c)] = word_new(85);
            break;
        case 68:
            memory[word_as_int(c)] = word_new(33);
            break;
        case 69:
            memory[word_as_int(c)] = word_new(112);
            break;
        case 70:
            memory[word_as_int(c)] = word_new(74);
            break;
        case 71:
            memory[word_as_int(c)] = word_new(83);
            break;
        case 72:
            memory[word_as_int(c)] = word_new(55);
            break;
        case 73:
            memory[word_as_int(c)] = word_new(50);
            break;
        case 74:
            memory[word_as_int(c)] = word_new(70);
            break;
        case 75:
            memory[word_as_int(c)] = word_new(104);
            break;
        case 76:
            memory[word_as_int(c)] = word_new(79);
            break;
        case 77:
            memory[word_as_int(c)] = word_new(65);
            break;
        case 78:
            memory[word_as_int(c)] = word_new(49);
            break;
        case 79:
            memory[word_as_int(c)] = word_new(67);
            break;
        case 80:
            memory[word_as_int(c)] = word_new(66);
            break;
        case 81:
            memory[word_as_int(c)] = word_new(54);
            break;
        case 82:
            memory[word_as_int(c)] = word_new(118);
            break;
        case 83:
            memory[word_as_int(c)] = word_new(94);
            break;
        case 84:
            memory[word_as_int(c)] = word_new(61);
            break;
        case 85:
            memory[word_as_int(c)] = word_new(73);
            break;
        case 86:
            memory[word_as_int(c)] = word_new(95);
            break;
        case 87:
            memory[word_as_int(c)] = word_new(48);
            break;
        case 88:
            memory[word_as_int(c)] = word_new(47);
            break;
        case 89:
            memory[word_as_int(c)] = word_new(56);
            break;
        case 90:
            memory[word_as_int(c)] = word_new(124);
            break;
        case 91:
            memory[word_as_int(c)] = word_new(106);
            break;
        case 92:
            memory[word_as_int(c)] = word_new(115);
            break;
        case 93:
            memory[word_as_int(c)] = word_new(98);
            break;
        default:
            error("unreachable code");
    }
}

void load_program(Word * memory, const char * file_name)
{
    FILE * f = fopen(file_name, "r");
    if (f == NULL)
    {
        error("failed to open input file");
    }

    int index = 0;
    char c;
    while ((c = getc(f)) != EOF)
    {
        if (isspace(c))
        {
            continue;
        }

        if (index == POW_3_10)
        {
            error("input file is too long");
        }
        
        switch ((c + index) % 94)
        {
            case 4:
            case 5:
            case 23:
            case 39:
            case 40:
            case 62:
            case 68:
            case 81:
                memory[index] = word_new(c);
                break;
            default:
                error("illegal character in input file");
        }
        
        index++;
    }
    
    fclose(f);

    if (index < 2)
    {
        error("input file is too short");
    }
    
    for (; index < POW_3_10; index++)
    {
        memory[index] = crazy_operation(memory[index - 2], memory[index - 1]);
    }
}

void execute_program(Word * memory)
{
    Word c = word_new(0);
    Word d = word_new(0);
    Word a = word_new(0);

    while (1)
    {
        int instruction = word_as_int(memory[word_as_int(c)]);
        if (instruction < 33 || instruction > 126)
        {
            return;
        }

        switch ((word_as_int(c) + instruction) % 94)
        {
            case 4:
                c = memory[word_as_int(d)];
                break;
            case 5:
                putchar(word_as_int(a) % 256);
                break;
            case 23:
            {
                int got = getchar();
                a = (got != EOF) ? word_new(got) : word_new(POW_3_10 - 1);
                break;
            }
            case 39:
                a = (
                    memory[word_as_int(d)] = rotate_right(memory[word_as_int(d)])
                );
                break;
            case 40:
                d = memory[word_as_int(d)];
                break;
            case 62:
                a = (
                    memory[word_as_int(d)] = crazy_operation(a, memory[word_as_int(d)])
                );
                break;
            case 68:
                break;
            case 81:
                return;
            default:
                break;
        }

        encipher(memory, c);

        c = word_new((word_as_int(c) + 1) % POW_3_10);
        d = word_new((word_as_int(d) + 1) % POW_3_10);
    }
}

int main(int argc, char ** argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s file_name\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    Word * memory = malloc(sizeof(Word) * POW_3_10);
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
