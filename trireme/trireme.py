#!/usr/bin/env py

import argparse
from enum import StrEnum
from pathlib import Path
import sys
from typing import Final, NoReturn


class Instruction(StrEnum):
    # Memory pointer commands
    MOVE_UP = 'mu'
    MOVE_DOWN = 'md'
    # Unary operations on x
    ROTATE_X = 'rx'  # Leftward
    SHIFT_X = 'sx'  # Rightward
    CYCLE_X = 'cx'  # Least significant trit
    NEGATE_X = 'nx'
    # Copies between x and other sources
    LOAD_X = 'xl'
    STORE_X = 'xs'
    # Debug operations
    DUMP_X = 'xd'


def parse_arguments(argv: list[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(prog=argv[0])
    parser.add_argument('file_name', type=Path)
    return parser.parse_args(argv[1:])


def indicate_user_error(message: str) -> NoReturn:
    print(f'ERROR: {message}')
    sys.exit(1)


def parse_program(file_name: Path) -> list[Instruction]:
    source_code = file_name.read_text(encoding='utf-8')
    program: list[Instruction] = []
    for word in source_code.strip().split():
        try:
            program.append(Instruction(word))
        except ValueError:
            indicate_user_error(f'found invalid instruction {word!r}')
    return program


type Tryte = tuple[int, int, int, int, int]

ZERO: Final[Tryte] = (0, 0, 0, 0, 0)


def execute(program: list[Instruction]) -> None:
    x: Tryte = ZERO
    memory: dict[int, Tryte] = {}
    ptr = 0
    for instruction in program:
        match instruction:
            case Instruction.MOVE_UP:
                ptr += 1
            case Instruction.MOVE_DOWN:
                ptr -= 1
            case Instruction.ROTATE_X:
                x_list = list(x)
                x_list.append(x_list.pop(0))
                x = tuple(x_list)
                del x_list
            case Instruction.SHIFT_X:
                x_list = list(x)
                x_list.pop()
                x_list.insert(0, 0)
                x = tuple(x_list)
                del x_list
            case Instruction.CYCLE_X:
                x_list = list(x)
                trit = x_list.pop()
                trit = -1 if trit == 1 else trit + 1
                x_list.append(trit)
                x = tuple(x_list)
                del x_list
            case Instruction.NEGATE_X:
                x = tuple(-trit for trit in x)
            case Instruction.LOAD_X:
                x = memory.get(ptr, ZERO)
            case Instruction.STORE_X:
                memory[ptr] = x
            case Instruction.DUMP_X:
                ternary = ''.join(
                    ('T' if trit == -1 else str(trit)) for trit in x
                )
                decimal = 0
                for trit in x:
                    decimal = 3 * decimal + trit
                print(f'Register X: {ternary} (decimal {decimal})')
                del ternary, decimal
            case _:
                raise ValueError(f'unknown instruction {instruction!r}')


def main(argv: list[str]) -> None:
    args = parse_arguments(argv)
    program = parse_program(args.file_name)
    execute(program)


if __name__ == '__main__':
    main(sys.argv)
