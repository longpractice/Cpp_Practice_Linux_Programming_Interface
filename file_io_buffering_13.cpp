/*
 * file_io_buffering_13.cpp
 *
 *  Created on: Dec 9, 2017
 *      Author: yng
 */


#include "common.h"
#include <vector>


/*
 *without osync, the speed is ok, with higher buf size, better performance
==============================
without osync, 10m file size:
1024 buffer size

real	0m0.277s
user	0m0.004s
sys	0m0.248s


2048 buffer size

real	0m0.192s
user	0m0.000s
sys	0m0.172s


4096 buffer size

real	0m0.167s
user	0m0.000s
sys	0m0.128s


8192 buffer size

real	0m0.134s
user	0m0.000s
sys	0m0.108s


16384 buffer size

real	0m0.163s
user	0m0.000s
sys	0m0.140s


==============================
==============================
without osync, 100m file size:
1024 buffer size

real	0m0.283s
user	0m0.000s
sys	0m0.260s


2048 buffer size

real	0m0.218s
user	0m0.000s
sys	0m0.196s


4096 buffer size

real	0m0.151s
user	0m0.000s
sys	0m0.132s


8192 buffer size

real	0m0.137s
user	0m0.000s
sys	0m0.124s


16384 buffer size

real	0m0.173s
user	0m0.000s
sys	0m0.148s


==============================






with osync, the speed is much slower, with higher buf size, better performance
==============================
with osync, 10m file size:
1024 buffer size

real	0m47.759s
user	0m0.012s
sys	0m9.588s


2048 buffer size

real	0m28.501s
user	0m0.008s
sys	0m5.772s


4096 buffer size

real	0m14.047s
user	0m0.012s
sys	0m3.144s


8192 buffer size

real	0m7.123s
user	0m0.008s
sys	0m1.576s


16384 buffer size

real	0m3.564s
user	0m0.000s
sys	0m0.760s


==============================

*/














void time_stats_13_1(int argc, char *argv[])
{
	int inputFd, outputFd, openFlags;
	mode_t filePerms;
	ssize_t numRead;

	if (argc != 4 || strcmp(argv[1], "--help") == 0)
		throw EUsage("%s old-file new-file\n", argv[0]);
	/* Open input and output files */
	inputFd = open(argv[1], O_RDONLY | O_SYNC);
	if (inputFd == -1)
		throw EUsage("Opening file %s failed.", argv[1]);

	openFlags = O_CREAT | O_WRONLY | O_TRUNC | O_SYNC;
	filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |	S_IROTH | S_IWOTH ; /* rw-rw-rw- */
	outputFd = open(argv[2], openFlags, filePerms);
	if (outputFd == -1)
		throw EUsage("Opening file %s failed", argv[2]);

	int buf_size = atoi(argv[3]);
	std::vector<char> buf_vec(buf_size);
	auto buf = buf_vec.data();

	/* Transfer data until we encounter end of input or an error */
	while ((numRead = read(inputFd, buf, buf_size)) > 0)
		if (write(outputFd, buf, numRead) != numRead)
			throw std::runtime_error("Couldn't write whole buffer to output file.");
	if (numRead == -1)
		throw std::runtime_error("Read does not finish.");
	if (close(inputFd) == -1)
		throw std::runtime_error("Close input failed.");
	if (close(outputFd) == -1)
		throw std::runtime_error("Close output failed.");
	return;

}
