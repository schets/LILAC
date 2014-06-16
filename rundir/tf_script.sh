#!/bin/sh
fout=$( printf 'lilac_output/run%04d.out' $TF_TASKID)
stdfout=$( printf 'stdout/run%04d.out' $TF_TASKID)
mkdir -p $SCRATCH/lilac_output
./lilac $SCRATCH/$fout $TF_TASKID 
