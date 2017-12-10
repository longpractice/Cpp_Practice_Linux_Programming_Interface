#!/bin/bash

echo "Exercise 13-2 compare bytes writing with different buf size and different file size"


echo "=============================="
echo "10m file size:"

for size in 1024 2048 4096 8192 16384
do
	sync
	echo 3 > /proc/sys/vm/drop_caches 	
	echo "$size buffer size"
	% TIMEFORMAT='%5R'; time ./Practices_Linux_Programming res 10240 $size
	echo
	echo
	rm res
done
echo "=============================="



echo "=============================="
echo "100m file size:"

for size in 1024 2048 4096 8192 16384
do
	sync
	echo 3 > /proc/sys/vm/drop_caches 	
	echo "$size buffer size"
	% TIMEFORMAT='%5R'; time ./Practices_Linux_Programming res 102400 $size
	echo
	echo
	rm res
done
echo "=============================="

