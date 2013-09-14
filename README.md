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
initial state. Such a file is provided in data/gosper_gun.dat. The run_life.sh
shell command demonstrates running life with this data file.

The data file is of the following format::

    toroidal flag
    milliseconds of wait time between generations
    width
    heigth
    zeros and ones for living and dead cells. Each line should be as wide as
    <width> and there should be as many lines as <height>

See data/gosper_gun.dat for an example

Authors
-------
Caleb Smith
