File formats for gameoflife
===========================

There are two types of data files for working with gameoflife. These are the
"seed" data files and the "rule" files. The seed data files determine the
beginning state (or seed) a particular execution of the simulation should have.
These also define a width, height, and a rule file that should be used. The
rule files define what states an automata can have, how to display them, and
the set of rules governing their transitions from one state to another.

A third type of file for working with gameoflife is a Scheme script. This acts
in much the same way as the transition rules of a rule file, but allow for
extensibility outside of the confines of a "pure" automata. In other words, a
cell's next state can be programmed in any way the author thinks is suitable
given the current state or any prior state that might have been recorded or
aggregated in a Scheme data structure, such as a heatmap. However, with only
the transitions of a rule file, an automata's cells use only tallies of
neighboring cells to determine their next state.

Each of these three types of files are described in further detail below

Seed data files
---------------
The data files that provide an initial state of the board have the following
format

    rule_filename
    width,height
    state:x,y
    state:x,y
    state:x,y
    ...

The following is an example data file

    life
    8,8
    1:4,4
    1:3,4
    1:3,3

Would use the rules/life.rule rule file (for Game of Life) and have the general
starting appearance of:

    00000000
    00000000
    00000000
    00010000
    00011000
    00000000
    00000000
    00000000

See data/gosper_gun.dat for a more practical example of a GOL automata
See data/diode.dat for a barebones example of WireWorld automata

Rule data files
---------------
The data files that provide the set of rules for an automata have the following
format

    number of states
    state number:character for text display:RGB hex color for graphical display
    ...(until all states are defined)
    scheme filename:scheme module:scheme function (or 0 for no scheme)
    neighbor counting scheme (moore or von_neumann)
    number of transition rules
    starting state->ending state:state to count in neighbor tally
    comma-separated list of neighbor tallies that make the transition happen
    ...(until all transitions are defined)

If a Scheme filename, module and function are defined, the remaining lines for
the transition definitions are moot and should be left off. Also, the
transition rule can be negated by prepending the "state to count in neighbor tally"
number with a ~. In other words, This transition will happen *unless* the tally
matches the counts specified.

For these files, examples are more instrutive. The following is an example for
Conway's Game of Life (taken from rules/life.rule)

    2
    0: :000000
    1:o:FFFFFF
    0
    moore
    2
    0->1:1
    3
    1->0:~1
    2,3

There are 2 states, 0 and 1, which will be displayed as " " and "o" in the text
UI. In the graphical UI, the 0 state will be black while the 1 state will be
white. Scheme is not needed, so the next line contains only a 0. The "moore"
line defines the neighbor counting procedure that will be used, which is the
"moore neighbors". These are the 8 neighbors, which includes diagonals.
A rule file with "von_neumann" will only count the 4 neighbors (no diagonals).

The remaining parts of this file define the transitions of this ruleset. There
are 2 transition rules. The first maps 0 to 1 (dead to alive, or a "birth").
This occurs when there are 3 neighbors with a state of 1. The next transition
causes a "death" of living cell (maps 1 to 0) unless there are 2 or 3 neighbors
that are living. In other words, a cell "survives" if it has 2 or 3 living
neighbors.

All cells remain in their current state in the next generation if they do not
match any of the transitions. Transition rules are applied in the order they
are defined.

To demonstrate an automata that uses Scheme, the rules/life_scm.rule file
implements the same behavior in Scheme, and has no transitions defined. Its
Scheme definition line is:

    scm/gameoflife.scm:gameoflife:get-next-cell

This will load the gameoflife.scm Scheme file in the scm folder (relative to
the gameoflife executable in the root project directory). The module will need
to export a module named "gameoflife" which defines a function named
"get-next-cell".

For a look at a different automata's rule file, the Brian's brain and
WireWorld automata are defined in rules/brain.rule and rules/wireworld.rule
respectively.

Scheme files
------------

Scheme files are defined in /scm. To use a Scheme file, define the filename
to be loaded, the module it exports, and the function to be called for each
cell as described in the Scheme example of the rule file documentation.

This function will be called with the x,y coordinates of the each cell in turn
and must return the value of the next state of the cell. Once returned, no
check or coercion is made to the state that the Scheme procedure returns, so
take care to ensure it is valid.

If the filename cannot be found, the module is not exported or the function
is not defined, the program will crash and display the error that the was
reached when attempting to load the Scheme file.

Refer to the [Scheme API Docs](scheme_api.md) for information on the
Scheme API
