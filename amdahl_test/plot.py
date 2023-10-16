#!/usr/bin/python3
#
# log format:
# "amdahl test result: pthread_num=1, serial_ratio=10, working_time=1, work_num=155637"
#
# data struct:
#
# 1. dictionary A: key(pthread_num)->value(working_num)
# 2. dictionary B: key(serial_ratio)-> dictionary A

# prepare data
data = {}

import re
re_t = re.compile(r"pthread_num=(\d+)")
re_r = re.compile(r"serial_ratio=(\d+)")
re_w = re.compile(r"work_num=(\d+)")

with open("log") as f:
    for line in f.readlines():
        s_rat = re_r.search(line)
        serial_ratio = s_rat.group(1)

        t_num = re_t.search(line)
        pthread_num = t_num.group(1)

        w_num = re_w.search(line)
        work_num = w_num.group(1)

        if serial_ratio in data:
            data[serial_ratio][pthread_num] = work_num
        else:
            data[serial_ratio] = {}
            data[serial_ratio][pthread_num] = work_num

# plot
import matplotlib.pyplot as plt

for ratio in data.keys():
    plt.plot(data[ratio].keys(), data[ratio].values(), label='serial_ratio: '+str(ratio)+'%')

plt.legend()
plt.title("amdahl test")
plt.xlabel("thread numbers")
plt.ylabel("work numbers")
plt.grid()
plt.show()
#plt.savefig("amdahl.svg")
