#!/bin/bash

for i in `seq 1000`
do
	make clean
	make -j64
done
