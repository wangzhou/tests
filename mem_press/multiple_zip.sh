#/bin/bash

for i in `seq $1`
do
	numactl --cpubind=1 --membind=1 zip_sva_perf -b 8192 -s 8192000 -l 1000 &
done
