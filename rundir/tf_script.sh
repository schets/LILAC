#!/bin/bash
fout=$( printf 'lilac_output/run%04d.out' $TF_TASKID)
stdout=$( printf 'output/run%04d.out' $TF_TASKID)
stdout=$( printf 'output/run%04d.err' $TF_TASKID)
./lilac+pat $SCRATCH/$fout $TF_TASKID 1>$stdout 2>$stdout
