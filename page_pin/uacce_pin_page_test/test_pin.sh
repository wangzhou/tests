#!/bin/sh

./a.out &
PID=`pidof a.out`

for i in `seq 10`
do
	numastat -p $PID
	sleep 2
done

kill -9 $PID
