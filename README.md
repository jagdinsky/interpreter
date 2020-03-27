## INTERPRETER
This program is special author code interpreter written on C++.
> To compile file you need to run "Makefile" (in Linux-terminal) from the main "interpreter/" directory.
> To run program you need to enter (in Linux-terminal) "./bin/interpreter < input/test.txt".

Our author code is written in "input/test.txt" file. It has following syntax features:
1) Code ends with an "exit" line:
    > exit
1) Program contains ASSIGN operator:
    > :=
2) There are only two data types: integer variables (int32) and arrays of integer variables.
    1. Variables are initialized by consecutives alphabetic letters and ASSIGN operator:
    > x := 3
    2. Arrays are initialized by consecutives letters and rectangular brackets:
    > arr[3]
    Array "arr[n]" is initialized for (n + 1) element, you can change value of the last element while initializing:
    > arr[3] = 5
    Now "arr" contains {0, 0, 0, 5}.
3) Program supports following operators:
    1. Left bracket
    > (
    2. Right bracket
    > )
    3. Logical OR
    > OR
    4. Logical AND
    > AND
    4. Equal
    > ==
    5. Not equal
    > !=
    6. Greater or equal
    > >=
    7. Greater
    > >
    8. Less or equal
    > <=
    9. Less
    > <
    10. Plus
    > +
    11. Minus
    > -
    12. Multiply
    > *
    13. Result of integer division
    > /
    14. Remainder of integer division
    > %
4) Interpreter supports straight code execution as well as conditional expressions and loops.
    1. "if", "else", "endif":
    > x := 0
    > y := 2
    > if x > y
    >     y := 1
    > else
    >     y := 2
    > endif
    > exit
    Now "y" is 2, because in this case "if x > y" means "if 0" or "if false".
    2. "while", "endwhile":
    > x := 8
    > y := 3
    > while x > y
    >     x := x - 2
    > endwhile
    > exit
    Now "x" is 2.
