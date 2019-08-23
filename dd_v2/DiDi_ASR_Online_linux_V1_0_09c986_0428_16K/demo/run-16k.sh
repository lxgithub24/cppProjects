#!/bin/sh 
make
split  -b 333698  test10-16k.pcm
./main2 xaa
./main2 xab
ls xa*  -lha | awk '{print $9}' > list.txt
./main1
