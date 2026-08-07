#!/usr/bin/env py
"""A brainf*ck compiler that uses transpilation to C.

Using a C compiler to produce the actual machine code is kind of cheating, but
I don't want to write the assembly myself. This script parses brainf*ck source
code from the provided input file, transpiles it to a series of C macros, and
inserts the macros into a template C program. Once transpilation is complete,
the C code gets automatically fed to a C compiler that produces an executable.
"""

import argparse
from enum import StrEnum
from pathlib import Path
import subprocess
import sys
from typing import Final, NoReturn


class Instruction(StrEnum):
    """All eight brainf*ck instructions as a string enumeration.
    """
    MOVE_RIGHT = '>'
    MOVE_LEFT = '<'
    INCREMENT = '+'
    DECREMENT = '-'
    PUT_CHAR = '.'
    GET_CHAR = ','
    LOOP_START = '['
    LOOP_END = ']'


# For typing purposes, a "program" is an immutable sequence of instructions
type Program = tuple[Instruction, ...]


# Table of optimizations that can be used to shorten brainf*ck programs
OPTIMIZATIONS: Final[dict[str, str]] = {
    '><': '',
    '<>': '',
    '+-': '',
    '-+': '',
}


# Path to the template C program file for transpilation
TEMPLATE_C_FILE: Final[Path] = Path(__file__).parent / 'template.c'

# Indentation used for main() function inside the template C file
INDENTATION: Final[str] = 4 * ' '

# String to find and replace in template C file when transpiling
INSERT_HERE: Final[str] = INDENTATION + '/* INSERT INSTRUCTIONS HERE */'

# Name of the C compiler to use and a sequence of flags to call it with
C_COMPILER: Final[str] = 'gcc'
C_COMPILER_FLAGS: Final[tuple[str, ...]] = (
    '-Wall', '-Wextra', '-Werror', '-O3',
)


def parse_args(argv: list[str]) -> argparse.Namespace:
    """Parse the command line arguments to this script using argparse.
    """
    parser = argparse.ArgumentParser(
        prog=argv[0],
        description='A brainf*ck compiler that uses transpilation to C.',
    )
    parser.add_argument('source_file', type=Path)
    parser.add_argument('-r', '--run', action='store_true')
    args = parser.parse_args(argv[1:])
    return args


def indicate_user_error(message: str) -> NoReturn:
    """Print a nice error message and terminate execution."""
    print(f'ERROR: {message}')
    sys.exit(1)


def read_program(source_file: Path) -> Program:
    """Return a sequence of brainf*ck instructions from a given source file.
    """
    if not source_file.is_file():
        indicate_user_error(
            f'source file does not exist: {str(source_file)!r}'
        )
    source_code = source_file.read_text(encoding='utf-8')
    program: list[Instruction] = []
    for character in source_code:
        try:
            program.append(Instruction(character))
        except ValueError:
            pass  # Skip any characters that are not valid instructions
    return tuple(program)


def validate(program: Program) -> None:
    """Report an error if the program is not syntactically valid."""
    brackets_opened = 0
    for instruction in program:
        match instruction:
            case Instruction.LOOP_START:
                brackets_opened += 1
            case Instruction.LOOP_END:
                if brackets_opened < 1:
                    indicate_user_error(
                        'found bracket mismatch:'
                        + ' program closes more loops than it opens'
                    )
                brackets_opened -= 1
            case _:
                # We're only concerned with checking that the loop brackets
                # nest properly, so ignore any other instructions.
                pass
    if brackets_opened > 0:
        indicate_user_error(
            'found bracket mismatch: program opens more loops than it closes'
        )


def optimize(program: Program) -> Program:
    """Optimize the program so that it requires fewer instructions."""
    program_as_str = ''.join(instruction.value for instruction in program)
    while True:
        length = len(program_as_str)
        for bad_str, better_str in OPTIMIZATIONS.items():
            program_as_str = program_as_str.replace(bad_str, better_str)
        if len(program_as_str) == length:
            break
    return tuple(Instruction(character) for character in program_as_str)


def transpile(program: Program, source_file: Path) -> Path:
    """Transpile a program to C and return the path to the created C file."""
    code_to_insert = '\n'.join(
        INDENTATION + instruction.name for instruction in program
    )
    template_code = TEMPLATE_C_FILE.read_text(encoding='utf-8')
    transpiled_code = template_code.replace(INSERT_HERE, code_to_insert)
    c_file = source_file.parent / (source_file.stem + '.c')
    c_file.write_text(transpiled_code, encoding='utf-8')
    return c_file


def run_c_compiler(c_file: Path) -> Path:
    """Run the C compiler on a C source file and return the output path."""
    command: list[str] = []
    command.append(C_COMPILER)
    command.extend(C_COMPILER_FLAGS)
    command.append(str(c_file))
    out_file = c_file.parent / (c_file.stem + '.out')
    command.extend(('-o', str(out_file)))
    subprocess.run(command, check=True)
    return out_file


def run_executable(out_file: Path) -> None:
    """Run the given executable file as if on the command line."""
    subprocess.run([str(out_file)], check=True)


def main(argv: list[str]) -> None:
    """Transpile brainf*ck source code to C and compile an executable."""
    args = parse_args(argv)
    program = read_program(args.source_file)
    validate(program)
    program = optimize(program)
    c_file = transpile(program, args.source_file)
    out_file = run_c_compiler(c_file)
    if args.run:
        run_executable(out_file)


if __name__ == '__main__':
    main(sys.argv)
