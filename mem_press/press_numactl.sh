#!/bin/bash

for i in `seq $1`
do
	numactl -C 32-47 --membind=1 ./a.out &
#	numactl -C 96-127 --membind=1 ./a.out &
	pid_array[$i]=$!
	echo ${pid_array[$i]}
done

echo -n "please enter 1 to kill test processes: "
read flag_1

for i in `seq $1`
do
	kill -9 ${pid_array[$i]}
done
