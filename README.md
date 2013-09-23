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

For Debian GNU/Linux these are provided in the following packages:

 * git
 * gcc
 * make
 * libncurses5-dev


Installation
------------
First, assure all requirements are met.
To run, clone the repo, run make in the src folder, then run 'run_life.sh'::

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

The data file is of the following format::

    width
    heigth
    zeros and ones for living and dead cells. Each line should be as wide as
        <width> and there should be as many lines as <height>

The following is an example data file::

    8
    8
    00000000
    01110100
    01000000
    00001100
    00110100
    01010100
    00000000
    00000000

See data/gosper_gun.dat for a more practical example

The command format is:

gameoflife [Options] seed_filename

Command line flags include:

* -t, --toroidal - Make the board toroidal (wrap around)
* -s number, --speed=number - Set the amount of microseconds between
    generations.


Authors
-------
Caleb Smith
