# ternary\_terminal

A little custom computer terminal using balanced ternary and minimal CPU instructions

## Sources of Inspiration

- RobCo terminals in the Fallout games (3, New Vegas, 4)
- Esoteric programming languages such as Brainf\*ck and Malbolge
- Balanced ternary, where possible trit values are -1, 0, or +1
- Programming on my TI-84 using a language similar to BASIC
- Assembly languages and binary machine code instructions

This isn't meant to be easy, just interesting.

## Balanced Ternary Computer

The balanced ternary computer will need something akin to a byte---a standard-sized collection of several trits so that I'm not having to fiddle with individual trits all the time.
Ideally, one of these will be sufficient to represent my character set using a system along the same lines as ASCII.
These "trytes" (name not final) should have enough trits to represent a decent number of characters, but I want the number of trits to be interesting as well.

- range with 3 trits: -13 to +13 (27 values)
- range with 4 trits: -40 to +40 (81 values)
- range with 5 trits: -121 to +121 (243 values)
- range with 6 trits: -364 to +364 (729 values)

I kind of like 5 since 81 is a decent number of total values and 5 is also prime, which might make things interesting and difficult.

## Other Text Snippets

Create a Fallout-style programmable RobCo terminal with a custom text display

My VM project idea needs -- an input method (keyboard?), an output method (terminal?), short-term storage (balanced trits in ram?), long-term storage?? (collections of trits in files?), an instruction queue (one-thread main program?), an assembler language, more advanced languages??, a system clock??

Write a computer algorithm that solves the Fallout 4 hacking minigame optimally. Check out that one project from the Big Book of Little Python Projects for help with simulating that minigame.


