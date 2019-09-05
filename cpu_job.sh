#!/bin/bash
export OMP_NUM_THREADS=2
start_time=$(date '+%s')
./sb 100
end_time=$(date '+%s')
elapsed_time=$((end_time - start_time))
sec=$((elapsed_time % 60))
min=$(((elapsed_time / 60) % 60))
hr=$((elapsed_time / 3600))
printf '%d:%02d:%02d' $hr $min $sec
echo

