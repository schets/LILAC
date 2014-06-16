/**
 * @page nersc_lilac Running on Nersc
 *
 * Nersc has published some good examples of jobs to run,
 * mainly <a href="https://www.nersc.gov/users/computational-systems/genepool/running-jobs/submitting-jobs/" here\a>
 *
 * In addition, one can find the script to run nersc jobs in rundir/nersc_run_script.
 * The script that runs LILAC is rundir/tf_script.sh. It is a fairly simple script.
 * It creates a filename that is attached to the index, called run#.out.
 * LILAC is then piped to the scratch folder, where the data is stored 
 *
 * To run with varying numbers of cores, one must edit the proper parameters in the script.
 * These are:
 *      - mppwidth: This is the total number of cores to reserve. Nodes come in chunks of 24 cores, so always reserve a multiple of 24
 *      - walltime: This is length of time the jobs is expected to run for. If you request too much, then the job will take longer to enter the queue.
 *      - -t: This is a command line option to the taskfarmer call. This is the number of jobs to start.
 *      - -n: This is another taskfarmer option which is the number of nodes to use. This should always be equal to mppwidth/24, unless jobs that each use multiple threads are ran.
 *
 * In addition, the queue type can be set to regular or debug.
 * The debug queue can only run smaller jobs and should only be used for testing.
 *
 * To submit a jub, perform
 * @code
 * qsub nersc-run-script.
 * @endcode
 * You can check the status of the job with
 * @code qstat -u kutz @endcode
 * and delete a job with
 * @code qdel <job-id> @endcode
 * The id can be found from the status command
 *
 * There is also a script called save_data. This will take everything in the folder
 * $SCRATCH/lilac_output and puts it into a .tgz file. It also prints the location of the file,
 * which will be in $scratch/final_data_outs/lilac-out.xxx.tgz. xxx consistof of two things. 
 * The first parameter passed to the script, and the second is the data.
 */
