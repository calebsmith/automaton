Scheme API
----------

To program an automaton in Scheme, an API is provided.  The following native
procedures for Scheme code to call are provided::

    (board-get-cell x y)
        Returns the state of the current cell at x, y

    (board-von-neuman-neighbors x y state)
        Returns the number of Von Neumann neighbors that have that state

    (board-moore-neighbors x y state)
        Returns the number of Moore neighbors that have that state

    (board-set-cell x y state)
        Set the state of the cell at x, y to have the value provided. This
        state is applied to the board being generated for the next generation,
        so the current board is unaffected. i.e. The result of this call will
        be observed in the next frame.

    (board-get-width)
        Returns the width of the current board

    (board-get-height)
        Returns the height of the current board

    (board-toroidal?)
        Returns one if the board is toroidal, otherwise 0.

    (board-done!)
        This procedure is provided to signal that all board-set-cell calls have
        been made. This is not necessary if the algorithm for the automaton
        visits each cell. However, if only some cells are set in each frame, as
        is the case for automata that have rules based on "actors" (such as
        Langton's Ant), the procedure should be called.

        While it is safe to call this procedure for automata that visit each
        cell, but it is unecessary and wasteful. The procedure copies the value
        of each cell in the next generation's board into that of the current
        one.


Refer to scm/gameoflife.scm for an example. This implements Conway's Game
of Life, which does not need Scheme, but it serves as a simple example.
