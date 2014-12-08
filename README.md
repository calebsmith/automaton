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

For Mac OS X, please see the section on [Installation for OS X](#install-osx)

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

If you are installing on Mac OS X, skip to [Installation for OS X](#install-osx).

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

Skip to [Usage](#usage) to start running this project.


<a name="install-osx"><a/>Installation for OS X
---------------------

This installation assumes that you are using homebrew and are using OS X
Yosemite. If you are using a different version or if you compile your tools
from scratch, or if you use another package manager such as MacPorts or pkgsrc,
the steps may be slightly different.

###Dependencies:

 * xcode
 * xcode command line tools
 * homebrew

Install the following via homebrew using 'brew install':

 * autoconf
 * automake
 * libtool
 * pkg-config
 * bdw-gc

Check the version of bdw-gc:

    brew info bdw-gc

If it says 7.5.0 or later, skip to [basic installation](#basic-osx-install). If
not, you most likely have a bug with xcode and cmake that will throw an
`unknown attribute __alloc_size__` error when you try to compile later. Follow
the workaround for homebrew and bdw-gc below.

###Workaround for Homebrew and bdw-gc:

The bdw-gc brew does not have a HEAD defined, so we have to install from source
and trick homebrew into thinking that it is using a brew.

> NOTE: This is using a non-stable, non-release commit for bdw-gc, and may
> contain bugs. The commit hashes used are the ones that worked during testing.
> Other hashes may work, but have not been tested.

Uninstall bdw-gc so it doesn't interfere with the source install. It will be
reinstalled again after the source install.

    brew uninstall bdw-gc

Choose a directory for downloading the bdw-gc source code and `cd` to it.

    git clone git://github.com/ivmai/bdwgc
    git checkout 0322b83386
    git clone git://github.com/ivmai/libatomic_ops
    cd libatomic_ops
    git checkout d200fdcc6e
    cd ..
    autoreconf -vif
    ./configure
    make && make install

Now reinstall bdw-gc. Ignore any messages about 'brew link' not completing
successfully.

    brew install bdw-gc

Finally, make brew used the compiled source:

    cd /usr/local/Cellar/bdw-gc/7.4.2/include
    rm -rf gc*
    cp -r /usr/local/include/gc* .
    cd ../share
    rm -rf gc
    cp -r /usr/local/include/gc .
    cd ../lib
    rm -f lib*
    find /usr/local/lib/libcord* -type f -exec cp -vp {} . \;
    find /usr/local/lib/libgc* -type f -exec cp -vp {} . \;
    ln -s libcord.1.dylib libcord.dylib
    ln -s libgc.1.dylib libgc.dylib
    brew link --overwrite bdw-gc

###<a name="basic-osx-install"></a>Basic Installation:

Install glfw:

    brew tap homebrew/versions
    brew install glfw3

Install guile:

    brew install guile

Choose a download directory for automaton and `cd` to it:

    git clone git@github.com:calebsmith/automaton.git
    cd automaton

Edit the Makefile to remove librt from LIBS. Your LIBS line should look like:

    LIBS = `pkg-config --libs $$GUILE_LIB_FLAGS guile-2.0` -lncurses `pkg-config --libs --static glfw3`

Now build, and you're ready to run automaton.

    make

> The terminal version exhibits some strange behavior on Mac, so automaton
> seems to run best with the -g (graphical) flag. The graphical version does
> take a lot of processing power and may freeze temporarily on Yosemite Macs
> with less than 8 GB of RAM.


<a name="usage"></a>Usage
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
