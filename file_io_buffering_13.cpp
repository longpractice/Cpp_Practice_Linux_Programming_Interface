/*
 * file_io_buffering_13.cpp
 *
 *  Created on: Dec 9, 2017
 *      Author: yng
 */


#include "common.h"
#include <vector>

#ifndef BUF_SIZE /* Allow "cc -D" to override definition */
#define BUF_SIZE 1024
#endif

/*
 *For file with 10m, constant time improvement real time and sys time till 4096, anything more give no significant improvement,
 *
yng@ubuntu:~$ fallocate -l 10m test

yng@ubuntu:~$ time ./Practices_Linux_Programming test res 1024

real	0m0.028s
user	0m0.000s
sys		0m0.024s

yng@ubuntu:~$ time ./Practices_Linux_Programming test res 2048

real	0m0.023s
user	0m0.004s
sys		0m0.016s
yng@ubuntu:~$ time ./Practices_Linux_Programming test res 4096

real	0m0.016s
user	0m0.000s
sys		0m0.012s
yng@ubuntu:~$ time ./Practices_Linux_Programming test res 8192

real	0m0.020s
user	0m0.000s
sys		0m0.016s

yng@ubuntu:~$ time ./Practices_Linux_Programming test res 16384

real	0m0.015s
user	0m0.000s
sys		0m0.012s

*/












/*
 *File size 1000m, same experiment. It is improved from 1024 - 8192 buffer size, then no obvious improvement.
 *
 *
yng@ubuntu:~$ fallocate -l 1000m largetestfile


yng@ubuntu:~$ time ./Practices_Linux_Programming largetestfile res 1024

real	0m1.798s
user	0m0.028s
sys		0m1.744s



yng@ubuntu:~$ time ./Practices_Linux_Programming largetestfile res 2048

real	0m1.228s
user	0m0.028s
sys		0m1.176s




yng@ubuntu:~$ time ./Practices_Linux_Programming largetestfile res 4096

real	0m0.949s
user	0m0.008s
sys		0m0.888s




yng@ubuntu:~$ time ./Practices_Linux_Programming largetestfile res 8192

real	0m0.859s
user	0m0.004s
sys	0m0.812s




yng@ubuntu:~$ time ./Practices_Linux_Programming largetestfile res 16384

real	0m0.927s
user	0m0.012s
sys		0m0.848s





yng@ubuntu:~$ time ./Practices_Linux_Programming largetestfile res 32768

real	0m0.990s
user	0m0.000s
sys		0m0.936s



 *
 */



void time_stats_13_1(int argc, char *argv[])
{
	int inputFd, outputFd, openFlags;
	mode_t filePerms;
	ssize_t numRead;

	if (argc != 4 || strcmp(argv[1], "--help") == 0)
		throw EUsage("%s old-file new-file\n", argv[0]);
	/* Open input and output files */
	inputFd = open(argv[1], O_RDONLY);
	if (inputFd == -1)
		throw EUsage("Opening file %s failed.", argv[1]);

	openFlags = O_CREAT | O_WRONLY | O_TRUNC;
	filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |	S_IROTH | S_IWOTH; /* rw-rw-rw- */
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
