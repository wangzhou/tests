#/bin/bash

for i in `seq $1`
do
	zip_sva_perf -b 8192 -s 81920000 &
done
