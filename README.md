LILAC: Learning and Integration of Lasers for Adaptive Control

This project provides a framework with which to analyze/control lasers, and more generally, any tunable dynamical system.

You can find documentation on the [wiki](http://github.com/schets/LILAC/wiki), and doxygen generated [advanced documentation](http://schets.github.io/LILAC) as well 

Installation:
This project depends on [FFTW](www.fftw.org), and the makefile has configurations for GCC and the Intel C++ Compiler. These are simple to install on Linux and Macintosh systems and each site provides installation instructions.

The library Eigen3 is included with the source. This is done to simplify installation, especially on NERSC which is iffy about non-standard libraries. Eigen3 is a set of headers, so no work is required on the part of the user.


Installation on Linux:

1. From the command line, proceed to the directory in which you want to have lilac installed.
2. Run the command: git clone https://github.com/schets/LILAC lilac
3. Enter the lilac directory (cd lilac)
4. Compile the code by running make
5. Upon successful compilation, the lilac binary can be found in the directory bin

Or, from to installation directory, copy and paste this into the command line

git clone https://github.com/schets/LILAC lilac && cd lilac && make


TODO (kinda ordered by priority):

1. Make all simulation/integrator classes type independent
2. Implement generic data storage and io modules
3. Implement neural network model
4. Implement stochastic integrators (possibly adaptive/implicit)
5. Implement template typelists and type black/whitelists for type_constructor class
6. Implement memory pool for aligned and 'local' memory
7. Remove uneeded dependencies, refactor code
8. Document configuration files
9. Find better method of transferring data from item* to value than void*
10. Allow referring to variables by name in input files
11. Allow passing specific parameters to variables
12. Write map-fold scripts for analyzing output
13. Refactor LSODA to run in integrator system
14. Get documentation up to date
