#!/usr/bin/env py

from pathlib import Path
import subprocess
import sys


PROGRAMS = Path(__file__).parent / 'programs'
COMPILER = Path(__file__).parent / 'brainf_ck.py'


if __name__ == '__main__':
    for source in PROGRAMS.glob('*.bf'):
        command = [COMPILER, source]
        print(*command)
        subprocess.run(command, check=True)
