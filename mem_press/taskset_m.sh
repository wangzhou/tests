#/bin/bash

for i in `seq $1`
do
	taskset 0xff000000000000 zip_sva_perf -b 8192 -s 8192000 -l 1000 -o perf &
done
