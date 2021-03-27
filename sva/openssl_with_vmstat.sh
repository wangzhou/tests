#!/bin/bash

~/scripts/diff_proc_vmstat_thp.sh 1
#./openssl_rsa_test.sh &> /tmp/uadk_log
./openssl_rsa_test.sh
~/scripts/diff_proc_vmstat_thp.sh 2
