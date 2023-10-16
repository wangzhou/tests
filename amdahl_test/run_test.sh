#!/bin/bash

echo > log

for r in $(seq 40 20 80)
do
	for n in $(seq 1 1 4)
	do
		./a.out --pthread_num=$n --serial_ratio=$r --working_time=1 >> log
	done
done
