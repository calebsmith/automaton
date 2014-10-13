automaton
==========

A simple cellular automata simulation runner in C
(such as Conway's Game of Life)
![automaton](http://i.imgur.com/LMw8CsK.gif)
![briansbraingraphical](http://i.imgur.com/OEZeJu6.png)

Quick Start
-----------
For 64-bit Ubuntu Linux, a tarball with precompiled binary and required assets
is provided for convenience in [automaton/releases](https://github.com/calebsmith/automaton/releases)

Notes
-----

This project is largely experimental and exists for my own learning, but has
recently become pretty stable nonetheless.

This was developed and tested on Ubuntu and Arch GNU/Linux. The curses and time
libraries rely on POSIX standards and will likely compile and run on Mac OSX
with some work, but this is not yet tested at all. (It is my suspicion that the
largest hurdles will be compiling the GLFW3 and Guile 2.0 dependencies).

Windows support is non-existent at the moment. If this is desired, please
contribute by making a pull request or opening an issue with links to good
documentation.

Dependencies
------------

 * ncurses 5.9.6
 * opengl
 * glfw 3.0.4
 * guile 2.0

For Arch these are provided in the following packages:

 * glfw
 * ncurses
 * guile

For Debian/Ubuntu GNU/Linux these are provided in the following packages:

 * libncurses5-dev
 * libglu1-mesa-dev
 * guile-2.0-dev
 * (must compile and install glfw 3 from source)

The base requirements for compiling are:

 * git 1.7.9
 * make 3.8.1
 * gcc 4.6.3

Redistribution
--------------

A `create_build.sh` shell script is available for making redistribution of the
program simpler. It is used to create the tarballs provided on the GitHub
releases page.

Currently, precompiled releases are provided for 64-bit Ubuntu only. Any help
with packaging on other distros is greatly appreciated.

At the moment, the release builds are not static linked. Any help in this
regard would be greatly appreciated.


Installation
------------
If you are on 64-bit Ubuntu, download a release from
[automaton/releases](https://github.com/calebsmith/automaton/releases)
You will likely need libguile-2.0, libncurses5 and libglu1-mesa-dev for
dynamic linking to work.

Otherwise, you'll need to build from source, at least for the moment. First,
ensure all requirements are met.

To build, clone the repo, run make in the src folder, then run any of the
shell scripts beginning with 'run', such as 'run_life.sh' to test it::


    git clone git@github.com:calebsmith/automaton.git
    cd automaton/src
    make
    cd ..
    source run_life.sh


Usage
-----

The program requires one argument, which is the data file for loading the
initial state (e.g. seed). Such files are provided with the repository in
the /data folder. The run_life.sh shell command demonstrates running
automaton with the gosper_gun data file.

The command format is:

automaton [Options] seed_filename

Command line flags include:

* -g, --graphical - Use graphical mode (OpenGL rendering)
* -f, --fullscreen - Use fullscreen rendering rather than windowed (pair with
        -g option, otherwise has no effect)
* -t, --toroidal - Make the board toroidal (wrap around)
* -s number, --speed=number - Set the amount of microseconds between
    generations. (Default is 85000)

Examples

    #Fast, full-screen graphical display
    ./automaton -g -f -s 1000 data/gosper_gun.dat
    #Slow, toroidal textual display
    ./automaton -s 500000 data/r_pentomino.dat


Usage During Runtime
--------------------

During program execution, the following keys have the corresponding effects:

* escape or q - quit
* p - pause
* r - resume
* h / left-arrow - move view left
* j / down-arrow - move view down
* k / up-arrow - move view up
* l / right-arrow - move view right
* i - zoom in
* o - zoom out


To define your own automata and seeds refer to the docs on file formats and the Scheme API:

[File formats](docs/file_formats.md).

[Scheme](docs/scheme_api.md).


Authors
-------
Caleb Smith
