LILAC: Learning and Integration of Lasers for Adaptive Control

This project provides a framework with which to analyze/control lasers, and more generally, any tunable dynamical system.

You can find documentation on the [wiki](http://github.com/schets/LILAC/wiki), and doxygen generated [advanced documentation](http://schets.github.io/LILAC) as well 

Installation:
This project depends on [FFTW](www.fftw.org), and the makefile has configurations for GCC and the Intel C++ Compiler. These are simple to install on Linux and Macintosh systems and each site provides installation instructions.

Once you have installed FFTW, you can proceed to download and compile LILAC. LILAC can be downloaded without git by going to the github page and manually downloading the file, but using git as a version control is much more convinient and less error prone.

On Linux:

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
5. Get documentation up to date
6. Write tutorial files for classes besides rhs
7. Document configuration files
8. Allow referring to variables by name in input files
9. Allow passing specific parameters to variables
10. Write map/fold scripts for analyzing output
11. Refactor LSODA to run in integrator system
12. Remove uneeded dependencies, refactor code
