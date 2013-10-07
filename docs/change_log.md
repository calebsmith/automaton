Change log
==========

v0.0.4 (Oct 06 2013)
-------------------
    * Added an OpenGL front-end.
    * Added key handling for pausing, resuming and exiting the simulation
    * Made non-toroidal board larger than the display size to prevent the
        border of the screen from interfering with escaping cells
    * Improved exit codes and error reporting

v0.0.3
-------------------
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
