#!/bin/bash

if [ $(uname -m) == "aarch64" ]; then
	test=1234
	echo $test
fi
