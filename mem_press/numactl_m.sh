#/bin/bash

for i in `seq $1`
do
	numactl -C 48-55 --membind=1 zip_sva_perf -b 8192 -s 8192000 -l 1000 -o perf &
done
