[
    This is a basic "truth machine" program written in brainf*ck.

    It reads a single character of input from the user. If the input was '0',
    then it prints one '0' and stops execution. If the input was '1', it goes
    into an infinite loop where it prints '1' over and over again.

    If the input is anything else, it prints nothing and stops execution.
]

Store the original input character in cell # minus 1
< ,

Copy the value of the input character to both cells #0 and #1
[ - > + > + << ]

Subtract 48 from both cells since 48 is the ASCII code for a 0
Cell # minus 1 equals zero now so we can use it as a loop counter
Note that 48 is equal to 8 times 6
+++ +++ ++ [ - > --- --- > --- --- << ]

Subtract 1 more from cell #1 since 49 is the ASCII code for a 1
>> -

At this point we have the following
Cell # minus 1 is 0 yet again
Cell #0 equals 0 only if the input was a 0
Cell #1 equals 0 only if the input was a 1

Next we need to effectively invert those as booleans
Set new flags in cell #2 and #3 to 1
> + > +

If cell #0 does not equal 0 then the input was not a 0
We will turn off the flag in cell #2
Also clear whatever value cell #0 does contain
<<< [ [-] >> - << ]

If cell #1 does not equal 0 then the input was not a 1
We will turn off the flag in cell #3
Also clear whatever value cell #1 does contain
> [ [-] >> - << ]

At this point cells #2 and #3 are the keys
Cell #2 is a 1 or 0 boolean indicating if the input was 0
Cell #3 is a 1 or 0 boolean indicating if the input was 1

If the flag in cell #2 is set then print a single 0
Construct the 0 for output using zeroed cells #0 and #1
> [
    -  Clear the flag to be tidy
    << +++ +++ ++ [ - > +++ +++ < ] > . [-]  Construct and output a 0
    +++ +++ +++ + . [-] >  Output a newline just to be fancy
]

If the flag in cell #3 is set then start an infinite loop of printing 1
Construct the 1 for output using zeroed cells #0 and #1
> [
    <<< +++ +++ ++ [ - > +++ +++ < ] > +  Construct a 1
    [.]  Infinite loop of output
]

If we reach this point then the input was not a 1
Either the input was 0 and we already output a 0
Or the input was something else and we just terminate here
