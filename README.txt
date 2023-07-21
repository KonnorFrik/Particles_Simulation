Atoms have:
    SDL_rect struct
    color in 'R G B A' format
    rule?


The rule defines how group of atoms interact with other group and with themselves

Atom struct have array with size of count of groups with powers which depends what do whith other groups

Example:
    2 groups - RED and GREEN
    RED attracted to RED
    RED repelled from GREEN

    GREEN neutral to GREEN
    GREEN attracted to RED

    RED have arr [-1, 0.5] // -1 for red atoms, 0.5 for green

    GREEN have arr [-0.3, 0] // -0.3 for red, 0 for green 

So: Color is enum
power position in arr is enum color code of atom struct


Key Map:
NumPad '+' - Plus size of atoms
NumPad '-' - Minus size of atoms
Escape - close programm

