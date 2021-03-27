#/bin/bash

for k in `seq 1`
do
	for i in `seq $1`
	do
	#	/home/wangzhou/perf stat -e hisi_sccl3_ddrc3/flux_wr/ numactl -C 48-55 --membind=1 zip_sva_perf -b 8192 -s 8192000 -l 1000 -o perf &
	#	numactl -C 48-55 --membind=1 zip_sva_perf -b 8192 -s 8192000 -l 1000 -o perf &
		zip_sva_perf -b 8192 -s 81920000 -l 1000 -o t &
	done
	
	wait
	sleep 2
done
