#!/bin/bash

numactl -C 32-47 --membind=1 /usr/lib/lmbench/bin/stream -P 10 -M 1024M -N 100
