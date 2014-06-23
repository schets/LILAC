LILAC: Learning and Integration of Lasers for Adaptive Control

This project provides a framework with which to analyze/control lasers, and more generally, any tunable dynamical system.

You can find advanced documentation [here](http://schets.github.io/LILAC)

Installation:
This project depends on [ACML](http://developer.amd.com/tools-and-sdks/cpu-development/cpu-libraries/amd-core-math-library-acml/acml-downloads-resources/#download) and [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page), and the makefile has configurations for GCC, CLANG, and the Intel C++ Compiler. These are simple to install on Linux and Macintosh systems and each site provides installation instructions.

Installation on Linux:

1. From the command line, proceed to the directory in which you want to have lilac installed.
2. Run the command: git clone https://github.com/schets/LILAC lilac
3. Enter the lilac directory (cd lilac)
4. Compile the code by running make
5. Upon successful compilation, the lilac binary can be found in the directory bin

Or, from to installation directory, copy and paste this into the command line

git clone https://github.com/schets/LILAC lilac && cd lilac && make


TODO (kinda ordered by priority):

1. Re-verify rhs_CNLS
2. Get documentation up to date
3. Add proper list semantics and combinations/permutations
4. Remove uneeded dependencies, refactor code (especially interpreter and engine/engineimp class)
5. Allow basic numerical operations to be done with lilac-lisp
6. Add functions and macros to lilac-lisp
7. Implement effecient way to reserve many different dimensions in the mempool
8. Write map-fold scripts for analyzing output
9. Refactor LSODA to run in integrator system
