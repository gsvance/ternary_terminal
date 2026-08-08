#!/usr/bin/env py

import argparse
import collections
from enum import StrEnum
from pathlib import Path
import random
import sys
from typing import NoReturn


def parse_arguments(argv: list[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(prog=argv[0])
    parser.add_argument('file_name', type=Path)
    return parser.parse_args(argv[1:])


def indicate_user_error(message: str) -> NoReturn:
    print(f'ERROR: {message}')
    sys.exit(1)


class Instruction(StrEnum):
    # Memory pointer commands
    MOVE_UP = 'mu'
    MOVE_DOWN = 'md'
    # Unary operations on x
    ROTATE_X = 'rx'  # Leftward
    SHIFT_X = 'sx'  # Rightward
    CYCLE_X = 'cx'  # Ones trit
    NEGATE_X = 'nx'
    # Copies between x and other sources
    LOAD_X = 'xl'
    STORE_X = 'xs'
    # Debug operations
    DUMP_X = 'xd'


def parse_program(file_name: Path) -> list[Instruction]:
    source_code = file_name.read_text(encoding='utf-8')
    program: list[Instruction] = []
    for line in source_code.splitlines():
        line_without_comments = line.split('#', maxsplit=1)[0]
        for word in line_without_comments.strip().split():
            try:
                program.append(Instruction(word))
            except ValueError:
                indicate_user_error(f'found invalid instruction {word!r}')
    return program


type Tryte = tuple[int, int, int, int, int]


def random_tryte() -> Tryte:
    return (
        random.randint(-1, 1),
        random.randint(-1, 1),
        random.randint(-1, 1),
        random.randint(-1, 1),
        random.randint(-1, 1),
    )


def rotate_left(tryte: Tryte) -> Tryte:
    t0, t1, t2, t3, t4 = tryte
    return t1, t2, t3, t4, t0


def shift_right(tryte: Tryte) -> Tryte:
    t0, t1, t2, t3, _t4 = tryte
    return 0, t0, t1, t2, t3


def cycle_ones_trit(tryte: Tryte) -> Tryte:
    t0, t1, t2, t3, t4 = tryte
    return t0, t1, t2, t3, (-1 if t4 == 1 else t4 + 1)


def negate(tryte: Tryte) -> Tryte:
    t0, t1, t2, t3, t4 = tryte
    return -t0, -t1, -t2, -t3, -t4


def dump_str(tryte: Tryte) -> str:
    ternary = ''.join(('T' if t == -1 else str(t)) for t in tryte)
    decimal = 0
    for t in tryte:
        decimal = 3 * decimal + t
    return f'{ternary} (decimal {decimal})'


def execute(program: list[Instruction]) -> None:
    x: Tryte = random_tryte()
    memory: collections.defaultdict[int, Tryte] = collections.defaultdict(
        random_tryte
    )
    ptr = 0
    for instruction in program:
        match instruction:
            case Instruction.MOVE_UP:
                ptr += 1
            case Instruction.MOVE_DOWN:
                ptr -= 1
            case Instruction.ROTATE_X:
                x = rotate_left(x)
            case Instruction.SHIFT_X:
                x = shift_right(x)
            case Instruction.CYCLE_X:
                x = cycle_ones_trit(x)
            case Instruction.NEGATE_X:
                x = negate(x)
            case Instruction.LOAD_X:
                x = memory[ptr]
            case Instruction.STORE_X:
                memory[ptr] = x
            case Instruction.DUMP_X:
                print(f'Register X: {dump_str(x)}')
            case _:
                raise ValueError(f'unknown instruction {instruction!r}')


def main(argv: list[str]) -> None:
    args = parse_arguments(argv)
    program = parse_program(args.file_name)
    execute(program)


if __name__ == '__main__':
    main(sys.argv)
