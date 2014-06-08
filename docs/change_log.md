Change log
==========

v0.0.8 (June 8 2014)
-------------------
    * Added documentation for file formats and Scheme API
    * Added support for defining automata rules in Scheme.
    * Uses pkg-config for glfw3 linker flags to make builds on different
        distributions simpler

v0.0.7 (May 11 2014)
--------------------
    * Upgraded glfw to version 3
    * Added a "world" module to simplify loading and passing boards and rules
        data
    * Improved Makefile to use object files
    * Fixed a bug to make the error when no filename is given print correctly
    * Fixed some bad calls to free() on program exit under some conditions
        (was crashing on exit for some rulesets)

v0.0.6 (Oct 18 2013)
--------------------
    * Removed hard coded Game of Life algorithm and replaced with a general
        "rule" file to define the automata to use.
    * Added some example data files for WireWorld automata.
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

