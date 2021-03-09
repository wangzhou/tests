#!/bin/bash

for i in `seq $1`
do
	taskset -c 32-47 ./a.out &
	pid_array[$i]=$!
	echo ${pid_array[$i]}
done

echo -n "please enter 1 to kill test processes: "
read flag_1

for i in `seq $1`
do
	kill -9 ${pid_array[$i]}
done
