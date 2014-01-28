LILAC: Learning and Integration of Lasers for Adaptive Control

This project provides a framework with which to analyze/control lasers, and more generally, any tunable dynamical system.

You can find documentation on the [wiki](http://github.com/schets/LILAC/wiki), and doxygen generated [advanced documentation](http://schets.github.io/LILAC) as well 

Installation:
This project depends on [GCC](gcc.gnu.org), [Git](git-scm.com), [FFTW](www.fftw.org) and [Eigen](eigen.tuxfamily.org). These are simple to install on Linux and Macintosh systems and each site provides installation instructions.

Once you have installed GCC, Git, FFTW, and Eigen, you can proceed to download and compile LILAC. LILAC can be downloaded without git by going to the github page and manually downloading the file, but using git as a version control is much more convinient and less error prone.

On Linux:

1. From the command line, proceed to the directory in which you want to have lilac installed.
2. Run the command: git clone https://github.com/schets/LILAC lilac
3. Enter the lilac directory (cd lilac)
4. Compile the code by running make
5. Upon successful compilation, the lilac binary can be found in ithe directory bin


See the wiki for a tutorial on writing input files, and extending the engine in various manners. The engine is designed so that only a cursory knowledge of C/C++ is needed to write most extensions, and in general extending the code will not involve editing the engine itself.

A nice tutorial on C++ can be found at www.cplusplus.com/tutorial, and a tutorial on pointers (arrays in C/C++) can be found at http://pw1.netcom.com/~tjensen/ptr/pointers.htm
