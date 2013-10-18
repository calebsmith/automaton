Change log
==========

v0.0.6 (Unreleased)
--------------------
    * Added interactive zooming to the lens feature.

v0.0.5 (Oct 13 2013)
--------------------
    * Added a "lens" to handle the display of the board. Allows for boards
        larger than the display surface, and handles logic for zooming,
        navigation, centering and so on.
    * Added navigation of the "lens" across the board using hjkl (ala vim) and
        arrow keys
    * Changed file format of seed data files to allow for easier creation of
        large sparse boards

v0.0.4 (Oct 06 2013)
--------------------
    * Added an OpenGL front-end.
    * Added key handling for pausing, resuming and exiting the simulation
    * Improved exit codes and error reporting
    * Split program into main, board, backend, textual_ui, and graphical_ui
        source files

v0.0.3
------
    * Made non-toroidal board extend beyond the visible bounds, allowing e.g.
        for escaping gliders to leave the visible board before colliding with
        the boundary of the board in memory
    * Several small code style improvements.
    * Improved text display (made cursor invisible, replaced 0 with o.)
    * Made toroidal board and speed command line options rather than loading
        these from data files.

v0.0.2
------
    * Added curses requirement and made text display consistent
    * Fixed CPU waiting to use POSIX nanosleep rather than the deprecated
        usleep.

v0.0.1
------
    * Initial public release

