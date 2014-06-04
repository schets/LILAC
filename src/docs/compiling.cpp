/**
 * @page compiling_page Compiling the Code
 *
 * Compiling LILAC is fairly simple.
 * First, make sure that the proper compiler is selected in the file make.inc. This is done by setting the variable CPP.
 *      - GCC is the standard compiler, and has the best performance on both the CNLS and C Elegans equations
 *      - Clang generally compiles fastest, and is the most standard conformant. I would reccomend using it for all but simulation builds
 *      - ICC is the Intel compiler. Generally, it outperforms GCC ut doesn't seem to do so in this case. I try to keep compatibility in case it starts outperforming GCC, or the MKL is needed for a function.
 *
 * The last value which CPP is set to will be the active compiler
 * One caveat is compiling on NERSC. In that case, set CPP to NERSC.
 *
 * Both GCC and Clang have full C++11 compliance, and Intel compiler 14.0.22 has very good compliance. 
 *
 * The binary will be in the directory bin, under the name lilac.
 */
