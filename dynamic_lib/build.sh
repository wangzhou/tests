#!/bin/bash

# build libdriver.so
	gcc -shared -fPIC -o libdriver.so driver_dl.c

# build libapi.so
	gcc -shared -fPIC -o libapi.so api_dl.c
	sudo cp libapi.so /usr/lib

# build app
	gcc -o app app.c -L. -lapi -ldl
