[
    Recite the letters of the alphabet in order from A to Z.

    Is brainf*ck as capable as the average kindergartner? Run this code and
    find out!

    The way that ASCII is organized actually makes this task relatively
    painless, so we do some fancy things along the way to keep it interesting.
]

Cell # minus 1 holds a counter that will start at 26
Note that 26 equals 4 times 6 plus 2
<< +++ + [ - > +++ +++ < ] > ++

Cell #0 will hold the capital letter we are currently outputting
Set that value to 65 initially since 65 is A in ASCII
Note that 65 equals 10 times 6 plus 5
>> +++ +++ +++ + [ - < +++ +++ > ] < +++ ++

Cell #1 will hold the lowercase letter we are currently outputting
Set that value to 97 initially since 97 is a in ASCII
Note that 97 equals 10 times 10 minus 3
>> +++ +++ +++ + [ - < +++ +++ +++ + > ] < ---

Cell #2 will hold the space character for nicer output
Set that value to 32 since that is the space character in ASCII
Note that 32 equals 4 times 8
>> +++ + [ - < +++ +++ ++ > ] <

Move back to the counter cell and begin the main loop
<<< [
    > . > .  Output from both the cells holding the letters
    + < + <  Increment each letter to the next one for the next loop
    -        Decrement the loop counter

    Copy the loop counter into cell # minus 2
    [ - < + < + >> ] << [ - >> + << ]

    If the copy of the loop counter is nonzero then output a space
    This means we are not on the very last loop where we output Zz
    > [ >>> > . <<< < [-] ] >

    < [-] >  Zero the counter copy if the previous line did not do it
]

Output a newline to make things a little nicer
+++ +++ +++ + .
