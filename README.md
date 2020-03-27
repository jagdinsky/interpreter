## INTERPRETER
This program is special author code interpreter written on C++.
1. To compile file you need to run Makefile by
> make

(in Linux-terminal) from the main "interpreter/" directory.

2. To run program you need to enter
> ./bin/interpreter < input/test.txt

(in Linux-terminal) from the main "interpreter/" directory.

User's code for interpreting is written in "input/test.txt" file. It has following syntax features:
1) Code ends with an "exit" line:
    > exit
1) Program contains ASSIGN operator:
    > :=
2) There are only two data types: integer variables (int32) and arrays of integer variables.
    1. Variables are initialized by consecutives alphabetic letters and ASSIGN operator:
    > x := 3
    2. Arrays are initialized by consecutives letters and rectangular brackets. Array "arr[n]" is initialized for (n + 1) element, you can change value of the last element while initializing. After this code "arrone" will contain {0, 0, 0} and "arrtwo" will contain {0, 0, 0, 0, 6}.
  
    > arrone[2]
    
    > arrtwo[4] = 6

3) Program supports following operators:
    
    | OPERATOR | SIGN |
    | ----------- | ----------- |
    | Left bracket | ( |
    | Right bracket | ) |
    
    3. Logical OR
    
    > OR
    
    4. Logical AND
    
    > AND
    
    5. Equal
    
    > ==
    
    6. Not equal
    
    > !=
    
    7. Greater or equal
    
    > >=
    
    8. Greater
    
    > >
    
    9. Less or equal
    
    > <=
    
    10. Less
    
    > <
    
    11. Plus
    
    > +
    
    12. Minus
    
    > -
    
    13. Multiply
    
    > *
    
    14. Result of integer division
    
    > /
    
    15. Remainder of integer division
    
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
