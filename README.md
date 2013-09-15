gameoflife
==========

A simple implementation of Conway's Game of Life in C


Requirements
------------

 * git 1.7.9
 * gcc 4.6.3
 * make 3.8.1


Installation
------------
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

    toroidal flag (0 for non-toroidal, 1 for toroidal)
    microseconds of wait time between generations
    width
    heigth
    zeros and ones for living and dead cells. Each line should be as wide as
        <width> and there should be as many lines as <height>

The following is an example data file::

    0
    55000
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

Authors
-------
Caleb Smith
