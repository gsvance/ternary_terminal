[
    Write all input from stdin directly to stdout.

    This version of a `cat` program in brainf*ck relies on an implementation
    detail where EOF writes a value of 0 to the memory cell. That's how my C
    template is set up to handle EOF, so this program will work correctly.
]

,  Get one character from stdin
[  While the input character is not zero (aka EOF)
    .  Write the character to stdout
    ,  Get another character from stdin
]  Loop back and check for EOF again
