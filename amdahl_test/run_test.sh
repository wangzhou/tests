#!/bin/bash

cat /dev/null > log

for r in $(seq 20 30 80)
do
	for n in $(seq 1 1 4)
	do
		./a.out --pthread_num=$n --serial_ratio=$r --working_time=2 >> log
	done
done
