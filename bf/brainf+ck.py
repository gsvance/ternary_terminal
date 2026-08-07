#!/usr/bin/env py

import argparse
from enum import StrEnum
from pathlib import Path
import subprocess
import sys
from typing import Final


class Instruction(StrEnum):
    MOVE_RIGHT = '>'
    MOVE_LEFT = '<'
    INCREMENT = '+'
    DECREMENT = '-'
    PUT_CHAR = '.'
    GET_CHAR = ','
    LOOP_START = '['
    LOOP_END = ']'


type Program = tuple[Instruction, ...]

TEMPLATE_FILE: Final[Path] = Path(__file__).parent / 'template.c'

INDENTATION: Final[str] = 4 * ' '
INSERT_HERE: Final[str] = INDENTATION + '/* INSERT INSTRUCTIONS HERE */'

C_COMPILER: Final[str] = 'gcc'
C_COMPILER_FLAGS: Final[tuple[str, ...]] = (
    '-Wall', '-Wextra', '-Werror', '-O3',
)


def parse_args(argv: list[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        prog=argv[0],
        description='A brainf*ck compiler that uses transpilation to C',
    )
    parser.add_argument('source_file', type=Path)
    args = parser.parse_args(argv[1:])
    return args


def read_program(source_file: Path) -> Program:
    source_code = source_file.read_text(encoding='utf-8')
    program: list[Instruction] = []
    for character in source_code:
        try:
            program.append(Instruction(character))
        except ValueError:
            pass  # Skip any characters that are not valid instructions
    return tuple(program)


def validate(program: Program) -> None:
    # TODO: raise errors if program contains mismatched brackets
    return


def optimize(program: Program) -> Program:
    # TODO: simplify the program by removing things like '><' or '+-'
    return program


def transpile(program: Program, source_file: Path) -> Path:
    code_to_insert = '\n'.join(
        INDENTATION + instruction.name for instruction in program
    )
    template_code = TEMPLATE_FILE.read_text(encoding='utf-8')
    transpiled_code = template_code.replace(INSERT_HERE, code_to_insert)
    c_file = source_file.parent / (source_file.stem + '.c')
    c_file.write_text(transpiled_code, encoding='utf-8')
    return c_file


def run_c_compiler(c_file: Path) -> None:
    command: list[str] = []
    command.append(C_COMPILER)
    command.extend(C_COMPILER_FLAGS)
    command.append(str(c_file))
    out_file = c_file.parent / (c_file.stem + '.out')
    command.extend(('-o', str(out_file)))
    subprocess.run(command, check=True)


def main(argv: list[str]) -> None:
    args = parse_args(argv)
    program = read_program(args.source_file)
    validate(program)
    program = optimize(program)
    c_file = transpile(program, args.source_file)
    run_c_compiler(c_file)


if __name__ == '__main__':
    main(sys.argv)
