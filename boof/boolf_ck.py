#!/usr/bin/env py

import enum
from pathlib import Path
import subprocess
import sys
from typing import Final


class Command(enum.StrEnum):
    MOVE_RIGHT = '>'
    MOVE_LEFT = '<'
    INCREMENT = '+'
    PUT_CHAR = ';'
    GET_CHAR = ','
    LOOP_START = '['
    LOOP_END = ']'


TEMPLATE_FILE: Final[Path] = Path(__file__).parent / 'boolf_template.c'

INSERT_HERE: Final[str] = '    /* INSERT INSTRUCTIONS HERE */'


def main(args: list[str]) -> None:
    _, infile = args
    commands: list[Command] = []
    with Path(infile).open('r') as f:
        for line in f:
            for c in line:
                if c in Command:
                    commands.append(Command(c))
    instructions = '\n'.join(
        ' ' * 4 + command.name for command in commands
    )
    template = TEMPLATE_FILE.read_text()
    transpiled = template.replace(INSERT_HERE, instructions, 1)
    outfile = TEMPLATE_FILE.parent / 'transpiled.c'
    outfile.write_text(transpiled)
    subprocess.run([
        'gcc', '-Wall', '-Wextra', '-Werror', '-O3',
        str(outfile), '-o', 'program.out',
    ])


if __name__ == '__main__':
    main(sys.argv)
