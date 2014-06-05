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
 * There is also a script called save_data. This will take everything in the folder
 * $SCRATCH/lilac_output and puts it into a .tgz file. It also prints the location of the file,
 * which will be in $scratch/final_data_outs/lilac-out.xxx.tgz. xxx consistof of two things. 
 * The first parameter passed to the script, and the second is the data.
 */
