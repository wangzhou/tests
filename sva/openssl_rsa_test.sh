#!/bin/bash

for i in `seq 1`
do
	#sudo openssl speed -elapsed -engine uadk -async_jobs 36 rsa2048
	#sudo openssl speed -elapsed -engine uadk -async_jobs 1 -seconds 1 rsa2048

	#sudo openssl rsautl -sign -in msg.txt -inkey pri.key -out signed.txt -engine uadk
	#sudo openssl rsautl -verify -in signed.txt -inkey pub.key -out verified.txt -engine uadk
	#cat verified.txt

	sudo openssl speed  -engine uadk  -seconds 1  rsa2048 
done
