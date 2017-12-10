#!/bin/bash


echo "=============================="
echo "without osync, 10m file size:"

fallocate -l 10m test
for size in 1024 2048 4096 8192 16384
do
	sync
	echo 3 > /proc/sys/vm/drop_caches 	
	echo "$size buffer size"
	time ./Practices_Linux_Programming_none_osync test res $size
	echo
	echo
	rm res
done
echo "=============================="



echo "=============================="
echo "without osync, 100m file size:"

fallocate -l 100m test
for size in 1024 2048 4096 8192 16384
do
	sync
	echo 3 > /proc/sys/vm/drop_caches 	
	echo "$size buffer size"
	time ./Practices_Linux_Programming_none_osync test res $size
	echo
	echo
	rm res
done
echo "=============================="







echo "=============================="
echo "with osync, 10m file size:"

fallocate -l 10m test
for size in 1024 2048 4096 8192 16384
do
	sync
	echo 3 > /proc/sys/vm/drop_caches 	
	echo "$size buffer size"
	time ./Practices_Linux_Programming test res $size
	echo
	echo
	rm res
done
echo "=============================="

