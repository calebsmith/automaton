Scheme API
----------

The heart of programming an automata with Scheme is using the provided API.
gameoflife defines the following native procedures for Scheme code to call

    (board-get-cell x y)
        Returns the state of the current cell at x, y
    (board-von-neuman-neighbors x y state)
        Returns the number of Von Neumann neighbors that have that state
    (board-moore-neighbors x y state)
        Returns the number of Moore neighbors that have that state

Refer to scm/gameoflife.scm for an example. This implements Conway's Game
of Life, which does not need Scheme, but it serves as a simple example.
