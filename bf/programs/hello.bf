[
    Print the string "Hello, world!" to stdout one character at a time.

    This is officially my first hand-written brainf*ck program! It could be
    written much more efficiently, but this works and that's what counts.
]

Go left by 6 cells so we use a few negative addresses
<<< <<<

Build 72 for H as 6 times 12
< +++ +++
[
    - >
    +++ +++ +++ +++
    <
]
>

Output H and move right
. >

Build 101 for e as 10 times 10 plus 1
< [-] +++ +++ +++ +
[
    - >
    +++ +++ +++ +
    <
]
> +

Output e and move right
. >

Build 108 for l as 9 times 12
< [-] +++ +++ +++
[
    - >
    +++ +++ +++ +++
    <
]
>

Output l twice and move right
.. >

Build 111 for o as 10 times 11 plus 1
< [-] +++ +++ +++ +
[
    - >
    +++ +++ +++ ++
    <
]
> +

Output o and move right
. >

Build 44 for comma as 6 times 7 plus 2
< [-] +++ +++
[
    - >
    +++ +++ +
    <
]
> ++

Output comma and move right
. >

Build 32 for space as 4 times 8
< [-] +++ +
[
    - >
    +++ +++ ++
    <
]
>

Output space and move right
. >

Build 119 for w as 10 times 12 minus 1
< [-] +++ +++ +++ +
[
    - >
    +++ +++ +++ +++
    <
]
> -

Output w and move right
. >

Once again build 111 for o as 10 times 11 plus 1
< [-] +++ +++ +++ +
[
    - >
    +++ +++ +++ ++
    <
]
> +

Output o and move right
. >

Build 114 for r as 6 times 19
< [-] +++ +++
[
    - >
    +++ +++ +++ +
    +++ +++ +++
    <
]
>

Output r and move right
. >

Yet again build 108 for l as 9 times 12
< [-] +++ +++ +++
[
    - >
    +++ +++ +++ +++
    <
]
>

Output l and move right
. >

Build 100 for d as 10 times 10
< [-] +++ +++ +++ +
[
    - >
    +++ +++ +++ +
    <
]
>

Output d and move right
. >

Build 33 for exclamation point as 4 times 8 plus 1
< [-] +++ +
[
    - >
    +++ +++ ++
    <
]
> +

Output exclamation point and move right
. >

Build 10 for newline
+++ +++ +++ +

Output newline
.

Clean up by zeroing final memory cell
[-]
