#!/bin/bash
fout=$( printf 'lilac_output/run%04d.out' $TF_TASKID)
stdout=$( printf 'output/lilac_stdout/run%04d.out' $TF_TASKID)
mkdir -p $SCRATCH/lilac_output
mkdir -p output
mkdir -p output/lilac_stdout
./lilac $SCRATCH/$fout $TF_TASKID 1>$stdout 2>$stdout
