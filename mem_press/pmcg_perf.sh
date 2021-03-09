#!/bin/bash

/home/wangzhou/perf stat -e smmuv3_pmcg_140020/0x8A/,filter_enable=1,filter_span=0,fillter_stream=0x75 <programm>
