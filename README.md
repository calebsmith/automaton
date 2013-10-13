gameoflife
==========

A simple implementation of Conway's Game of Life in C
![gameoflife](http://i.imgur.com/B0ElyTn.png)

Notes
-----

This project is experimental and exists for my own learning.

This was developed and tested on GNU/Linux. The curses and time libraries rely
on POSIX standards and will likely compile and run on Mac OSX, but this is not
yet tested.

Windows support is non-existent at the moment. If this is desired, please
contribute by making a pull request or opening an issue with links to good
documentation.


Requirements
------------

 * git 1.7.9
 * gcc 4.6.3
 * make 3.8.1
 * curses 5.9
 * opengl
 * glfw

For Debian GNU/Linux these are provided in the following packages:

 * git
 * gcc
 * make
 * libncurses5-dev
 * libgl1-mesa-dev
 * libglfw-dev


Installation
------------
First, assure all requirements are met.
To run, clone the repo, run make in the src folder, then run 'run_life.sh'

    git clone git@github.com:calebsmith/gameoflife.git
    cd gameoflife/src
    make
    cd ..
    source run_life.sh


Usage
-----

The program requires one argument, which is the data file for loading the
initial state (e.g. seed). Such files are provided with the repository in
the /data folder. The run_life.sh shell command demonstrates running
gameoflife with the gosper_gun data file.

The command format is:

gameoflife [Options] seed_filename

Command line flags include:

* -g, --graphical - Use graphical mode (OpenGL rendering)
* -f, --fullscreen - Use fullscreen rendering rather than windowed (pair with
        -g option, otherwise has no effect)
* -t, --toroidal - Make the board toroidal (wrap around)
* -s number, --speed=number - Set the amount of microseconds between
    generations. (Default is 85000)

Examples

    #Fast, full-screen graphical display
    ./gameoflife -g -f -s 1000 data/gosper_gun.dat
    #Slow, toroidal textual display
    ./gameoflife -s 500000 data/r_pentomino.dat


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


Seed data files
---------------
The data files that provide an initial state of the board have the following
format

    width,height
    state:x,y
    state:x,y
    state:x,y
    ...

The following is an example data file

    8,8
    1:4,4
    1:3,4
    1:3,3

Would have the general appearance of:

    00000000
    00000000
    00000000
    00010000
    00011000
    00000000
    00000000
    00000000

See data/gosper_gun.dat for a more practical example


Authors
-------
Caleb Smith
