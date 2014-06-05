/**
 * @page running_lilac Running Lilac
 *
 * LILAC is a fairly simple program to run. It takes few command line parameters, instead
 * utilizing input files to control program execution.
 *
 * It can take two input parameters-the name of the output file, and the index in that order.
 * Both are optional parameters, but one must specify the output file if the index is to be specified.
 *
 * If no input parameters are specified, the program runs with an output file of data_out.out,
 * and an index of zero. This is equivilant to a call like
 * ./lilac data_out.out 0
 *
 * If the output file is passed, then the output will be contained in the filename,
 * and if the index is passed, then LILAC will run at that index.
 *
 * When running at a non-zero index, LILAC does the following:
 *      -# It takes the number of iterations that a single run is supposed to have
 *      -# Finds out how many iterations should happen if a single core were to reach the current run#
 *      -# Performs that many fake iterations
 *
 * A fake iteration means that the engine moves forwards a step in the parameter space,
 * but doesn't perform a simulation or update any of the internal classes.
 *
 *
 * For running on distributed platforms, it is often best to run a script instead of directly running
 * LILAC. This script could create a custom filename for each index,
 * and then call LILAC with the proper parameters. Examples can be found in run_dir.
 */
