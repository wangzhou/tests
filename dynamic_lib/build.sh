#/bin/bash

if [ $1 = "d" ]; then
# build libdriver.so
	gcc -shared -fPIC -o libdriver.so driver_dl.c

# build libapi.so
	gcc -shared -fPIC -o libapi.so api_dl.c
	sudo cp libapi.so /usr/lib

# build app
	gcc -o app app.c -L. -lapi -ldl
fi

if [ $1 = "s" ]; then
# build libdriver.so
	gcc -o libdriver.o -c driver_dl.c -DSTATIC_DRV
	ar -rcs libdriver.a libdriver.o

# build libapi.so
	gcc -o libapi.o -c api_dl.c -DSTATIC_DRV
	ar -rcs libapi.a libapi.o

# build app
	gcc -o app app.c -L. -lapi -ldriver --static -DSTATIC_DRV
fi

if [ $1 = "c" ]; then
	rm app libapi.so libapi.a libdriver.so libdriver.a libapi.o libdriver.o
fi
